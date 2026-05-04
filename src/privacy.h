#ifndef NEBULA_PRIVACY_H
#define NEBULA_PRIVACY_H

#include "efi_min.h"

typedef struct {
    int installed;
    int privacy_strict;
    int vpn_enabled;
} PrivacyState;

void privacy_load(EFI_SYSTEM_TABLE *st, PrivacyState *state);
void privacy_persist(EFI_SYSTEM_TABLE *st, const PrivacyState *state);

#endif
