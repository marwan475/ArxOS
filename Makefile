
CC = x86_64-w64-mingw32-g++ \
		-Wl,--subsystem,10 \
		-e efi_main

LD = x86_64-w64-mingw32-ld

CFLAGS = \
	-Wall \
	-Wextra \
	-Wpedantic \
	-mno-red-zone \
	-ffreestanding \
	-nostdlib 

BIN = bin/
BUILD = build/
OUTPUT = TwistedOS.img

EFI = BOOTX64.EFI
IMG = $(OUTPUT)
ESP_SIZE = 64
SECTORS = $(shell echo $$(( $(ESP_SIZE) * 2048 )))

all: bin build $(IMG)

clean:
	rm -rf $(BIN) $(BUILD) $(OUTPUT)

build:
	mkdir $(BUILD)

bin:
	mkdir $(BIN)

$(EFI): Bootloader/bootloader.cpp
	$(CC) $(CFLAGS) -I. -I./Bootloader -o $(BIN)$@ $< \
		-L /usr/lib -l:libefi.a -l:libgnuefi.a

$(IMG): $(EFI)
	rm -f $@
	dd if=/dev/zero of=$@ bs=512 count=$(SECTORS) status=none
	parted $@ --script mklabel gpt
	parted $@ --script mkpart ESP fat32 1MiB 100%
	parted $@ --script set 1 boot on
	START=$$(parted -s $@ unit s print | awk '/^ 1/ {print $$2}' | sed 's/s//'); \
	ESP=esp.fat; \
	dd if=/dev/zero of=$$ESP bs=512 count=$(SECTORS) status=none; \
	mkfs.fat -F32 $$ESP >/dev/null; \
	export MTOOLS_SKIP_CHECK=1; \
	mmd -i $$ESP ::/EFI; mmd -i $$ESP ::/EFI/BOOT; \
	mcopy -i $$ESP $(BIN)$(EFI) ::/EFI/BOOT/BOOTX64.EFI; \
	dd if=$$ESP of=$@ bs=512 seek=$$START conv=notrunc status=none; \
	rm -f $$ESP

qemu-uefi:
	qemu-system-x86_64 -bios UEFI64.bin -drive file=TwistedOS.img,format=raw

#qemu-system-x86_64 -bios UEFI64.bin -net none   -drive file=TwistedOS.img,format=raw -device virtio-gpu-pci -display gtk -full-screen
#ATI Rage 128 Pro ati-vga

CHANGED_FILES := $(shell git diff --name-only --diff-filter=ACMRTUXB HEAD | grep -E '\.(c|cpp|h|hpp)$$')

.PHONY: format
format:
	@if [ -n "$(CHANGED_FILES)" ]; then \
		echo "Formatting changed files:"; \
		echo "$(CHANGED_FILES)"; \
		clang-format -i --style="{ \
			BasedOnStyle: llvm, \
			IndentWidth: 4, \
			TabWidth: 4, \
			UseTab: Never, \
			ColumnLimit: 100, \
			BreakBeforeBraces: Allman, \
			AllowShortIfStatementsOnASingleLine: false, \
			AllowShortLoopsOnASingleLine: false, \
			AllowShortFunctionsOnASingleLine: None, \
			AllowShortBlocksOnASingleLine: Never, \
			PointerAlignment: Left, \
			ReferenceAlignment: Left, \
			AlignOperands: true, \
			AlignConsecutiveAssignments: true, \
			AlignConsecutiveDeclarations: true, \
			AlignTrailingComments: true, \
			AlignAfterOpenBracket: Align, \
			BreakBeforeBinaryOperators: All, \
			SpaceBeforeParens: ControlStatements, \
			SpacesInParentheses: false, \
			SpacesInSquareBrackets: false, \
			SpacesInAngles: false, \
			SpaceAfterCStyleCast: true, \
			SpaceBeforeAssignmentOperators: true, \
			KeepEmptyLinesAtTheStartOfBlocks: false, \
			SortIncludes: true, \
			IncludeBlocks: Regroup, \
			NamespaceIndentation: None, \
			AccessModifierOffset: -4, \
			IndentCaseLabels: true, \
			BreakConstructorInitializersBeforeComma: false, \
			BreakInheritanceList: BeforeColon, \
			ConstructorInitializerIndentWidth: 4, \
			ContinuationIndentWidth: 8, \
			ReflowComments: true, \
			SpacesBeforeTrailingComments: 1, \
			Cpp11BracedListStyle: true \
		}" $(CHANGED_FILES); \
	else \
		echo "No changed C/C++ files."; \
	fi
