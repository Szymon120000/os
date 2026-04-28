#include "gui.h"

static void draw_chip_text(Gfx *gfx, UINT32 x, UINT32 y, UINT32 chars, UINT32 color) {
    for (UINT32 i = 0; i < chars; i++) {
        gfx_fill(gfx, x + i * 7, y, 5, 2, color);
    }
}

static UINT32 app_color(UINT16 kind, const Gfx *gfx) {
    if (kind == 1) return gfx_rgb(gfx, 0x59, 0x7f, 0xd1);  // browser
    if (kind == 2) return gfx_rgb(gfx, 0x8f, 0x6c, 0xd4);  // notes
    if (kind == 3) return gfx_rgb(gfx, 0x5e, 0xb1, 0x84);  // files
    if (kind == 4) return gfx_rgb(gfx, 0xd9, 0x8c, 0x2b);  // installer
    return gfx_rgb(gfx, 0x66, 0x7a, 0x90);
}

void gui_render_desktop(Gfx *gfx, int installed) {
    gfx_gradient(gfx, 0x303845, 0x171c25);

    UINT32 top = gfx_rgb(gfx, 0x12, 0x17, 0x21);
    UINT32 dock = gfx_rgb(gfx, 0x12, 0x17, 0x21);

    gfx_fill(gfx, 0, 0, gfx->width, 32, top);
    gfx_fill(gfx, 0, gfx->height - 48, gfx->width, 48, dock);

    UINT32 install_chip = installed ? gfx_rgb(gfx, 0x2f, 0x82, 0x4f) : gfx_rgb(gfx, 0x8a, 0x52, 0x30);
    gfx_fill(gfx, 18, 7, 180, 18, install_chip);

    for (UINT32 i = 0; i < 7; i++) {
        gfx_fill(gfx, 210 + i * 40, gfx->height - 34, 28, 20, gfx_rgb(gfx, 0x33, 0x4a, 0x64));
    }

    UINT32 shortcuts = installed ? 3 : 4;
    for (UINT32 i = 0; i < shortcuts; i++) {
        gfx_fill(gfx, 24 + i * 76, 54, 58, 58, gfx_rgb(gfx, 0x2a, 0x48, 0x6b));
        gfx_fill(gfx, 34 + i * 76, 64, 38, 38, gfx_rgb(gfx, 0x76, 0xa7, 0xde));
    }
}

void gui_launch_app(Gfx *gfx, const ExuImage *app, UINT32 slot) {
    if (!app || !app->header) return;

    ExuMeta meta = {0};
    if (!exu_parse_meta(app, &meta)) return;

    UINT32 ww = gfx->width > 1200 ? 540 : gfx->width / 2 - 40;
    UINT32 wh = gfx->height > 820 ? 350 : gfx->height / 2 - 50;

    UINT32 col = slot % 2;
    UINT32 row = slot / 2;
    UINT32 wx = 80 + col * (ww + 26);
    UINT32 wy = 150 + row * (wh + 24);

    if (wx + ww + 10 > gfx->width) wx = gfx->width > ww + 20 ? gfx->width - ww - 20 : 0;
    if (wy + wh + 10 > gfx->height) wy = gfx->height > wh + 60 ? gfx->height - wh - 60 : 0;

    gfx_fill(gfx, wx - 2, wy - 2, ww + 4, wh + 4, gfx_rgb(gfx, 0x0c, 0x0e, 0x12));
    gfx_fill(gfx, wx, wy, ww, wh, gfx_rgb(gfx, 0xf5, 0xf7, 0xfa));
    gfx_fill(gfx, wx, wy, ww, 42, app_color(app->header->app_kind, gfx));

    gfx_fill(gfx, wx + 14, wy + 10, ww - 28, 22, gfx_rgb(gfx, 0xff, 0xff, 0xff));
    gfx_fill(gfx, wx + 14, wy + 58, ww - 28, wh - 72, gfx_rgb(gfx, 0xff, 0xff, 0xff));

    UINT32 title_chars = (UINT32)(meta.name_len > 22 ? 22 : meta.name_len);
    UINT32 entry_chars = (UINT32)(meta.entry_len > 32 ? 32 : meta.entry_len);
    draw_chip_text(gfx, wx + 20, wy + 16, title_chars, gfx_rgb(gfx, 0x1a, 0x2d, 0x43));
    draw_chip_text(gfx, wx + 20, wy + 17, entry_chars, gfx_rgb(gfx, 0x35, 0x4d, 0x68));

    gfx_fill(gfx, wx + ww - 98, wy + 12, 20, 16, gfx_rgb(gfx, 0xe9, 0x6d, 0x77));
    gfx_fill(gfx, wx + ww - 72, wy + 12, 20, 16, gfx_rgb(gfx, 0xe1, 0xb5, 0x59));
    gfx_fill(gfx, wx + ww - 46, wy + 12, 20, 16, gfx_rgb(gfx, 0x7b, 0xcf, 0x73));

    gfx_fill(gfx, wx + 30, wy + 92, ww - 60, 3, gfx_rgb(gfx, 0x1f, 0x28, 0x34));
    gfx_fill(gfx, wx + 30, wy + 122, ww - 110, 2, gfx_rgb(gfx, 0x39, 0x47, 0x58));
    gfx_fill(gfx, wx + 30, wy + 148, ww - 150, 2, gfx_rgb(gfx, 0x39, 0x47, 0x58));
}
