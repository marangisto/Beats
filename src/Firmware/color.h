#pragma once

#include <color.h>
#include <fontlib.h>

static const color::color_t text_fg = color::yellow;
static const color::color_t text_bg = color::steel_blue;
static const color::color_t frame_fg = color::black;
static const color::color_t screen_bg = color::dark_slate_gray;

static const fontlib::font_t& font() { return fontlib::cmunvt_28; }

