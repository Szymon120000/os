#ifndef NEBULA_GFX_H
#define NEBULA_GFX_H

#include "efi_min.h"

typedef struct {
    UINT32 width;
    UINT32 height;
    UINT32 stride;
    UINT32 *fb;
    UINT32 pixel_format;
    UINT32 red_mask;
    UINT32 green_mask;
    UINT32 blue_mask;
} Gfx;

void gfx_init(Gfx *gfx, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);
UINT32 gfx_rgb(const Gfx *gfx, UINT32 r, UINT32 g, UINT32 b);
void gfx_fill(Gfx *gfx, UINT32 x, UINT32 y, UINT32 w, UINT32 h, UINT32 color);
void gfx_gradient(Gfx *gfx, UINT32 top_rgb, UINT32 bottom_rgb);

#endif
