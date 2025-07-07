
CC = x86_64-w64-mingw32-gcc \
		-Wl,--subsystem,10 \
		-e efi_main

LD = x86_64-w64-mingw32-ld

CFLAGS = \
	-std=c17 \
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


$(EFI): boot.c
	$(CC) $(CFLAGS) -I. -o $(BIN)$@ $< \
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
	qemu-system-x86_64 -bios UEFI64.bin -net none -drive file=TwistedOS.img,format=raw


