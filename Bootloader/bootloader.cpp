#include <uefi.hpp>
#include <Console.hpp>

Console *Con;

extern "C" void _putchar(char character){
    Con->putchar(character);
}

extern "C"
{
    EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
    {
        EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConsoleOutput = SystemTable->ConOut;

        Console Cons = Console(SystemTable->ConOut);
        Con = &Cons;
        
        Cons.Reset();
        Cons.ClearConsole();
        Cons.printf_("Welcome to ArxOS Bootloader\r\n");

        while (true)
        {
        }

        return 0;
    }
}
