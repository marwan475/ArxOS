#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab) {
    InitializeLib(image, systab);
    Print(L"Hello from UEFI!\n");
    return EFI_SUCCESS;
}

