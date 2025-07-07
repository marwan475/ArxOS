
#include "efi.h"

// EFI Image Entry Point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
 
  (void)ImageHandle;	// Prevent compiler warning
  SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Welcome to TwistedOS Uefi Bootloader \r\n\r\n");

  SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to shutdown..."); 

  // Wait until keypress, then return
  EFI_INPUT_KEY key;
  while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS);
   
  // Shutdown, does not return
  SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Should never get here
  return EFI_SUCCESS;
}

