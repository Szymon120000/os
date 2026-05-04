#ifndef EFI_MIN_H
#define EFI_MIN_H

#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

typedef uint64_t UINT64;
typedef uint32_t UINT32;
typedef uint16_t UINT16;
typedef uint16_t CHAR16;
typedef uint8_t UINT8;
typedef uint64_t UINTN;
typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void VOID;

#define EFI_SUCCESS 0
#define EFI_ERROR_MASK 0x8000000000000000ULL
#define EFI_ERROR(s) (((s) & EFI_ERROR_MASK) != 0)

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

typedef struct EFI_TABLE_HEADER {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
typedef struct EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

typedef EFI_STATUS(EFIAPI *EFI_TEXT_STRING)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    VOID *Reset;
    EFI_TEXT_STRING OutputString;
    VOID *TestString;
    VOID *QueryMode;
    VOID *SetMode;
    VOID *SetAttribute;
    VOID *ClearScreen;
    VOID *SetCursorPosition;
    VOID *EnableCursor;
    VOID *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);
typedef EFI_STATUS(EFIAPI *EFI_STALL)(UINTN Microseconds);
typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(UINTN Timeout, UINT64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);

typedef struct EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER Hdr;
    VOID *RaiseTPL;
    VOID *RestoreTPL;
    VOID *AllocatePages;
    VOID *FreePages;
    VOID *GetMemoryMap;
    VOID *AllocatePool;
    VOID *FreePool;
    VOID *CreateEvent;
    VOID *SetTimer;
    VOID *WaitForEvent;
    VOID *SignalEvent;
    VOID *CloseEvent;
    VOID *CheckEvent;
    VOID *InstallProtocolInterface;
    VOID *ReinstallProtocolInterface;
    VOID *UninstallProtocolInterface;
    VOID *HandleProtocol;
    VOID *PCHandleProtocol;
    VOID *RegisterProtocolNotify;
    VOID *LocateHandle;
    VOID *LocateDevicePath;
    VOID *InstallConfigurationTable;
    VOID *LoadImage;
    VOID *StartImage;
    VOID *Exit;
    VOID *UnloadImage;
    VOID *ExitBootServices;
    VOID *GetNextMonotonicCount;
    EFI_STALL Stall;
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
    VOID *ConnectController;
    VOID *DisconnectController;
    VOID *OpenProtocol;
    VOID *CloseProtocol;
    VOID *OpenProtocolInformation;
    VOID *ProtocolsPerHandle;
    VOID *LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL LocateProtocol;
    VOID *InstallMultipleProtocolInterfaces;
    VOID *UninstallMultipleProtocolInterfaces;
    VOID *CalculateCrc32;
    VOID *CopyMem;
    VOID *SetMem;
    VOID *CreateEventEx;
} EFI_BOOT_SERVICES;

typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 *Attributes, UINTN *DataSize, VOID *Data);
typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 Attributes, UINTN DataSize, VOID *Data);

typedef struct EFI_RUNTIME_SERVICES {
    EFI_TABLE_HEADER Hdr;
    VOID *GetTime;
    VOID *SetTime;
    VOID *GetWakeupTime;
    VOID *SetWakeupTime;
    VOID *SetVirtualAddressMap;
    VOID *ConvertPointer;
    EFI_GET_VARIABLE GetVariable;
    VOID *GetNextVariableName;
    EFI_SET_VARIABLE SetVariable;
} EFI_RUNTIME_SERVICES;

typedef struct EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER Hdr;
    CHAR16 *FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    VOID *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    VOID *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
    UINTN NumberOfTableEntries;
    VOID *ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
    UINT32 Version;
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    UINT32 PixelFormat;
    struct {
        UINT32 RedMask;
        UINT32 GreenMask;
        UINT32 BlueMask;
        UINT32 ReservedMask;
    } PixelInformation;
    UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    UINT32 MaxMode;
    UINT32 Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN SizeOfInfo;
    UINT64 FrameBufferBase;
    UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    UINT32 ModeNumber,
    UINTN *SizeOfInfo,
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    UINT32 ModeNumber);

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode;
    VOID *Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

#endif
