#include "efi_min.h"
#include "gfx.h"
#include "gui.h"
#include "exu.h"

#define PIXEL_RED_GREEN_BLUE_RESERVED8BIT_PER_COLOR 0
#define PIXEL_BLUE_GREEN_RED_RESERVED8BIT_PER_COLOR 1
#define PIXEL_BIT_MASK 2
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004

static EFI_GUID GOP_GUID = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
static EFI_GUID NEBULA_VAR_GUID = {0x6b3f7905, 0x4c29, 0x43a8, {0x93, 0x9f, 0xcf, 0xa4, 0xbc, 0x38, 0x10, 0x57}};
static CHAR16 INSTALLED_VAR_NAME[] = { 'N','e','b','u','l','a','I','n','s','t','a','l','l','e','d',0 };

static const uint8_t BROWSER_EXU[] = {
    'E','X','U','1',0x01,0x00,0x01,0x00,0x1c,0x00,0x00,0x00,
    'N','e','b','u','l','a',' ','B','r','o','w','s','e','r','|','n','e','b','u','l','a',':','/','/','h','o','m','e'
};
static const uint8_t NOTES_EXU[] = {
    'E','X','U','1',0x01,0x00,0x02,0x00,0x17,0x00,0x00,0x00,
    'Q','u','i','c','k',' ','N','o','t','e','s','|','n','o','t','e','s',':','/','/','t','o','d','a','y'
};
static const uint8_t FILES_EXU[] = {
    'E','X','U','1',0x01,0x00,0x03,0x00,0x18,0x00,0x00,0x00,
    'F','i','l','e','s',' ','V','i','e','w','|','f','i','l','e','s',':','/','/','h','o','m','e','/','u','s','r'
};
static const uint8_t INSTALLER_EXU[] = {
    'E','X','U','1',0x01,0x00,0x04,0x00,0x1a,0x00,0x00,0x00,
    'S','y','s','t','e','m',' ','I','n','s','t','a','l','l','e','r','|','i','n','s','t','a','l','l',':','/','/','n','o','w'
};

static EFI_GRAPHICS_OUTPUT_PROTOCOL *get_gop(EFI_SYSTEM_TABLE *st) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = 0;
    if (EFI_ERROR(st->BootServices->LocateProtocol(&GOP_GUID, 0, (VOID **)&gop))) return 0;
    return gop;
}

static int mode_is_usable(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info) {
    if (!info) return 0;
    if (info->PixelFormat != PIXEL_RED_GREEN_BLUE_RESERVED8BIT_PER_COLOR &&
        info->PixelFormat != PIXEL_BLUE_GREEN_RED_RESERVED8BIT_PER_COLOR &&
        info->PixelFormat != PIXEL_BIT_MASK) return 0;
    return 1;
}

static UINT32 mode_score(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info) {
    UINT32 w = info->HorizontalResolution;
    UINT32 h = info->VerticalResolution;

    if (w == 1920 && h == 1080) return 100000000u;
    if (w == 1366 && h == 768)  return 99000000u;
    if (w == 1600 && h == 900)  return 98000000u;
    if (w == 1280 && h == 720)  return 97000000u;
    if (w == 1280 && h == 800)  return 96000000u;
    if (w == 1024 && h == 768)  return 95000000u;
    if (w == 800  && h == 600)  return 94000000u;
    if (w == 640  && h == 480)  return 93000000u;

    return w * h;
}

static void pick_best_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
    UINT32 current = gop->Mode->Mode;
    UINT32 best_mode = current;
    UINT32 best_score = 0;

    for (UINT32 i = 0; i < gop->Mode->MaxMode; i++) {
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = 0;
        UINTN info_size = 0;
        if (gop->QueryMode(gop, i, &info_size, &info) != EFI_SUCCESS || !mode_is_usable(info)) continue;

        UINT32 score = mode_score(info);
        if (score > best_score) {
            best_score = score;
            best_mode = i;
        }
    }

    if (best_mode != current && gop->SetMode(gop, best_mode) != EFI_SUCCESS) {
        gop->SetMode(gop, current);
    }
}

static int is_installed(EFI_SYSTEM_TABLE *st) {
    if (!st || !st->RuntimeServices || !st->RuntimeServices->GetVariable) return 0;

    UINT8 installed = 0;
    UINTN data_size = sizeof(installed);
    EFI_STATUS status = st->RuntimeServices->GetVariable(INSTALLED_VAR_NAME, &NEBULA_VAR_GUID, 0, &data_size, &installed);

    return (!EFI_ERROR(status) && installed == 1) ? 1 : 0;
}

static void write_installed_flag(EFI_SYSTEM_TABLE *st) {
    if (!st || !st->RuntimeServices || !st->RuntimeServices->SetVariable) return;

    UINT8 installed = 1;
    st->RuntimeServices->SetVariable(
        INSTALLED_VAR_NAME,
        &NEBULA_VAR_GUID,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(installed),
        &installed);
}

static int parse_app(const uint8_t *bytes, UINTN size, ExuImage *out) {
    return exu_parse(bytes, (size_t)size, out);
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st) {
    (void)image;

    if (st && st->BootServices && st->BootServices->SetWatchdogTimer) {
        st->BootServices->SetWatchdogTimer(0, 0, 0, 0);
    }

    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = get_gop(st);
    if (!gop || !gop->Mode || !gop->Mode->Info) {
        st->ConOut->OutputString(st->ConOut, L"No GOP.\r\n");
        return 1;
    }

    pick_best_mode(gop);

    Gfx gfx;
    gfx_init(&gfx, gop);

    int installed = is_installed(st);

    ExuImage apps[4] = {0};
    UINT32 app_count = 0;

    if (!parse_app(BROWSER_EXU, sizeof(BROWSER_EXU), &apps[app_count++])) return 2;
    if (!parse_app(NOTES_EXU, sizeof(NOTES_EXU), &apps[app_count++])) return 3;
    if (!parse_app(FILES_EXU, sizeof(FILES_EXU), &apps[app_count++])) return 4;

    if (!installed) {
        if (!parse_app(INSTALLER_EXU, sizeof(INSTALLER_EXU), &apps[app_count++])) return 5;
    }

    gui_render_desktop(&gfx, installed);
    for (UINT32 i = 0; i < app_count; i++) {
        gui_launch_app(&gfx, &apps[i], i);
    }

    if (!installed) {
        write_installed_flag(st);
        st->ConOut->OutputString(st->ConOut, L"Live mode: installer completed. Reboot to installed mode.\r\n");
    }

    for (;;) st->BootServices->Stall(1000000);
    return EFI_SUCCESS;
}
