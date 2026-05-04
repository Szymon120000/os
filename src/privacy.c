#include "privacy.h"

#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004

static EFI_GUID NEBULA_VAR_GUID = {0x6b3f7905, 0x4c29, 0x43a8, {0x93, 0x9f, 0xcf, 0xa4, 0xbc, 0x38, 0x10, 0x57}};

static CHAR16 INSTALLED_VAR[] = { 'N','e','b','u','l','a','I','n','s','t','a','l','l','e','d',0 };
static CHAR16 PRIVACY_VAR[] = { 'N','e','b','u','l','a','P','r','i','v','a','c','y','S','t','r','i','c','t',0 };
static CHAR16 VPN_VAR[] = { 'N','e','b','u','l','a','V','p','n','E','n','a','b','l','e','d',0 };

static int read_flag(EFI_SYSTEM_TABLE *st, CHAR16 *name, int fallback) {
    if (!st || !st->RuntimeServices || !st->RuntimeServices->GetVariable) return fallback;

    UINT8 v = (UINT8)fallback;
    UINTN size = sizeof(v);
    EFI_STATUS s = st->RuntimeServices->GetVariable(name, &NEBULA_VAR_GUID, 0, &size, &v);
    if (EFI_ERROR(s)) return fallback;
    return v ? 1 : 0;
}

static void write_flag(EFI_SYSTEM_TABLE *st, CHAR16 *name, int value) {
    if (!st || !st->RuntimeServices || !st->RuntimeServices->SetVariable) return;

    UINT8 v = value ? 1 : 0;
    st->RuntimeServices->SetVariable(
        name,
        &NEBULA_VAR_GUID,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(v),
        &v);
}

void privacy_load(EFI_SYSTEM_TABLE *st, PrivacyState *state) {
    state->installed = read_flag(st, INSTALLED_VAR, 0);
    state->privacy_strict = read_flag(st, PRIVACY_VAR, 1);
    state->vpn_enabled = read_flag(st, VPN_VAR, 1);
}

void privacy_persist(EFI_SYSTEM_TABLE *st, const PrivacyState *state) {
    write_flag(st, INSTALLED_VAR, state->installed);
    write_flag(st, PRIVACY_VAR, state->privacy_strict);
    write_flag(st, VPN_VAR, state->vpn_enabled);
}
