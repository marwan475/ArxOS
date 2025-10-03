#include <uefi.hpp>

extern "C"
{
    EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
    {
        while(true){}

        return 0;
    }
}
