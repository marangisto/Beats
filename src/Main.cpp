#include <stdlib.h>
#include <cstdio>
#include <timer.h>
#include <gpio.h>
#include <st7789.h>
#include <text.h>

using namespace stm32f0;
using namespace timer;
using namespace gpio;
using namespace st7789;
using namespace fontlib;

typedef encoder_t<2, PA15, PB3> encoder;
//typedef encoder_t<3, PB4, PB5> encoder;
typedef st7789_t<1, PA5, PA7, PC5, PC4> display;

constexpr double pi = 3.14159265358979323846;

void loop(const font_t&, color_t, color_t);

int main()
{
    encoder::setup<pull_up>(1 + (5 << 1));
    display::setup();

    font_t ft = fontlib::font;
    const color_t fg = color::white;
    const color_t bg = color::black;

    display::clear(bg);

    for (;;)
        loop(ft, fg, bg);
}

void loop(const font_t& ft, color_t fg, color_t bg)
{
    text_renderer_t<display> tr(ft, fg, bg);

    tr.set_pos(50, 50);
    tr.write("Welcome to Beats!");

    for (;;)
    {
        char buf[128];

        sprintf(buf, "%05ld", encoder::count() >> 1);
        tr.set_pos(50, 100);
        tr.write(buf);
    }
}

