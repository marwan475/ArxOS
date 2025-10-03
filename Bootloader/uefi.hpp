// https://uefi.org/sites/default/files/resources/UEFI_Spec_2_10_Aug29.pdf

#include <stdint.h>

// Table 2.4
#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define EFIAPI __attribute__((ms_abi)) // Calling Spec UEFI Spec 2.3

// Data Types UEFI Spec 2.3.1
typedef uint8_t  BOOLEAN;
typedef int64_t  INTN;
typedef uint64_t UINTN;
typedef int8_t   INT8;
typedef uint8_t  UINT8;
typedef int16_t  INT16;
typedef uint16_t UINT16;
typedef int32_t  INT32;
typedef uint32_t UINT32;
typedef int64_t  INT64;
typedef uint64_t UINT64;
typedef char     CHAR8;
typedef char16_t CHAR16;
typedef void     VOID;

typedef struct
{
    UINT32 TimeLow;
    UINT16 TimeMid;
    UINT16 TimeHighAndVersion;
    UINT8  CLockSeqHighAndReserved;
    UINT8  ClockSeqLow;
    UINT8  Node[6];
} __attribute__((packed)) EFI_GUID;

typedef UINTN  EFI_STATUS;
typedef void*  EFI_HANDLE;
typedef void*  EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN  EFI_TPL;

typedef void EFI_SYSTEM_TABLE;

typedef EFI_STATUS(EFIAPI* EFI_IMAGE_ENTRY_POINT)(IN EFI_HANDLE        ImageHandle,
                                                  IN EFI_SYSTEM_TABLE* SystemTable);
