#include "exu.h"

int exu_parse(const uint8_t *bytes, size_t size, ExuImage *out) {
    if (!bytes || !out || size < sizeof(ExuHeader)) return 0;

    const ExuHeader *h = (const ExuHeader *)bytes;
    if (h->magic[0] != 'E' || h->magic[1] != 'X' || h->magic[2] != 'U' || h->magic[3] != '1') return 0;
    if (h->version != 1) return 0;
    if (sizeof(ExuHeader) + h->payload_size > size) return 0;

    out->header = h;
    out->payload = bytes + sizeof(ExuHeader);
    out->payload_size = h->payload_size;
    return 1;
}

int exu_parse_meta(const ExuImage *img, ExuMeta *out) {
    if (!img || !out || !img->header || img->payload_size < 3) return 0;

    const char *p = (const char *)img->payload;
    size_t split = 0;
    while (split < img->payload_size && p[split] != '|') split++;
    if (split == 0 || split + 1 >= img->payload_size) return 0;

    out->name = p;
    out->name_len = split;
    out->entry = p + split + 1;
    out->entry_len = img->payload_size - split - 1;
    return 1;
}
