#ifndef NEBULA_GUI_H
#define NEBULA_GUI_H

#include "gfx.h"
#include "exu.h"

void gui_render_desktop(Gfx *gfx, int installed, int privacy_strict, int vpn_enabled);
void gui_launch_app(Gfx *gfx, const ExuImage *app, UINT32 slot);

#endif
