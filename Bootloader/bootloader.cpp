#include <uefi.hpp>

extern "C"
{
    EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
    {

        EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConsoleOutput = SystemTable->ConOut;

        ConsoleOutput->Reset(ConsoleOutput, false);
        ConsoleOutput->ClearScreen(ConsoleOutput);
        ConsoleOutput->OutputString(ConsoleOutput, u"Welcome to ArxOS Bootloader\n");

        while(true){}

        return 0;
    }
}
