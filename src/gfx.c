#include "gfx.h"

#define PIXEL_RED_GREEN_BLUE_RESERVED8BIT_PER_COLOR 0
#define PIXEL_BLUE_GREEN_RED_RESERVED8BIT_PER_COLOR 1
#define PIXEL_BIT_MASK 2

static UINT32 channel_to_mask(UINT32 value, UINT32 mask) {
    if (mask == 0) return 0;

    UINT32 shift = 0;
    while (((mask >> shift) & 1u) == 0u && shift < 31) shift++;

    UINT32 width = 0;
    while (((mask >> (shift + width)) & 1u) == 1u && (shift + width) < 32) width++;

    if (width == 0) return 0;
    UINT32 max = (1u << width) - 1u;
    UINT32 scaled = (value * max) / 255u;
    return (scaled << shift) & mask;
}

void gfx_init(Gfx *gfx, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = gop->Mode->Info;
    gfx->width = info->HorizontalResolution;
    gfx->height = info->VerticalResolution;
    gfx->stride = info->PixelsPerScanLine;
    gfx->fb = (UINT32 *)(UINTN)gop->Mode->FrameBufferBase;
    gfx->pixel_format = info->PixelFormat;
    gfx->red_mask = info->PixelInformation.RedMask;
    gfx->green_mask = info->PixelInformation.GreenMask;
    gfx->blue_mask = info->PixelInformation.BlueMask;
}

UINT32 gfx_rgb(const Gfx *gfx, UINT32 r, UINT32 g, UINT32 b) {
    if (gfx->pixel_format == PIXEL_RED_GREEN_BLUE_RESERVED8BIT_PER_COLOR) {
        return ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
    }

    if (gfx->pixel_format == PIXEL_BLUE_GREEN_RED_RESERVED8BIT_PER_COLOR) {
        return ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu);
    }

    if (gfx->pixel_format == PIXEL_BIT_MASK) {
        return channel_to_mask(r, gfx->red_mask) |
               channel_to_mask(g, gfx->green_mask) |
               channel_to_mask(b, gfx->blue_mask);
    }

    return ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
}

void gfx_fill(Gfx *gfx, UINT32 x, UINT32 y, UINT32 w, UINT32 h, UINT32 color) {
    UINT32 x2 = x + w;
    UINT32 y2 = y + h;
    if (x2 > gfx->width) x2 = gfx->width;
    if (y2 > gfx->height) y2 = gfx->height;

    for (UINT32 yy = y; yy < y2; yy++) {
        for (UINT32 xx = x; xx < x2; xx++) {
            gfx->fb[yy * gfx->stride + xx] = color;
        }
    }
}

void gfx_gradient(Gfx *gfx, UINT32 top_rgb, UINT32 bottom_rgb) {
    UINT32 tr = (top_rgb >> 16) & 0xffu, tg = (top_rgb >> 8) & 0xffu, tb = top_rgb & 0xffu;
    UINT32 br = (bottom_rgb >> 16) & 0xffu, bg = (bottom_rgb >> 8) & 0xffu, bb = bottom_rgb & 0xffu;

    for (UINT32 y = 0; y < gfx->height; y++) {
        UINT32 r = tr + ((br - tr) * y / (gfx->height ? gfx->height : 1));
        UINT32 g = tg + ((bg - tg) * y / (gfx->height ? gfx->height : 1));
        UINT32 b = tb + ((bb - tb) * y / (gfx->height ? gfx->height : 1));
        UINT32 c = gfx_rgb(gfx, r, g, b);
        for (UINT32 x = 0; x < gfx->width; x++) gfx->fb[y * gfx->stride + x] = c;
    }
}
