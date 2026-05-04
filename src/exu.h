#ifndef NEBULA_EXU_H
#define NEBULA_EXU_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t magic[4];
    uint16_t version;
    uint16_t app_kind;
    uint32_t payload_size;
} ExuHeader;

typedef struct {
    const ExuHeader *header;
    const uint8_t *payload;
    size_t payload_size;
} ExuImage;

typedef struct {
    const char *name;
    size_t name_len;
    const char *entry;
    size_t entry_len;
} ExuMeta;

int exu_parse(const uint8_t *bytes, size_t size, ExuImage *out);
int exu_parse_meta(const ExuImage *img, ExuMeta *out);

#endif
