#include <stdlib.h>
#include <cstdio>
#include <timer.h>
#include <gpio.h>
#include <st7789.h>
#include <text.h>
#include "Euclidean.h"
#include "List.h"

using namespace stm32f0;
using namespace timer;
using namespace gpio;
using namespace st7789;
using namespace fontlib;

typedef encoder_t<2, PA15, PB3> encoder;
//typedef encoder_t<3, PB4, PB5> encoder;
typedef st7789_t<1, PA5, PA7, PC5, PC4> display;

constexpr double pi = 3.14159265358979323846;

void loop(text_renderer_t<display>& tr);

int main()
{
    encoder::setup<pull_up>(1 + (7 << 1));
    display::setup();

    font_t ft = fontlib::cmunss_24;
    const color_t fg = color::white;
    const color_t bg = color::black;

    display::clear(bg);
    text_renderer_t<display> tr(ft, fg, bg, true);

    tr.set_pos(50, 50);
    tr.write("Welcome to Beats!");

    for (;;)
        loop(tr);
}

void loop(text_renderer_t<display>& tr)
{
    static uint32_t last_count = -1;
    uint32_t count = encoder::count() >> 1;

    if (count != last_count)
    {
        char buf[128];
 
        sprintf(buf, "%05ld", count);
        tr.set_pos(50, 100);
        tr.write(buf);
        tr.set_pos(100, 100);

        auto xs = expand(euclidean(count + 1, 8));
        for (auto x : xs)
           tr.write(x ? '1' : '0');
        last_count = count;
    }
}

