#include "../Firmware/board.h"
#include <widget.h>

using hal::sys_tick;

void clock_trigger() {}
void reset_trigger() {}

int main()
{
    board::setup();

    const fontlib::font_t& font = fontlib::cmunvt_28;
    static valuebox_t<board::tft, show_int> box1, box2, box3, box4;
    static vertical_t<board::tft> col;
    static border_t<board::tft> panel;
    static color::color_t bg = color::steel_blue;
    static color::color_t fg = color::yellow;
    static bool quiet = false;

    box1.setup(font, fg, bg, 0, &quiet);
    box2.setup(font, fg, bg, 0, &quiet);
    box3.setup(font, fg, bg, 0, &quiet);
    box4.setup(font, fg, bg, 0, &quiet);
    col.setup();
    col.append(&box1);
    col.append(&box2);
    col.append(&box3);
    col.append(&box4);
    panel.setup(&col, color::black);
    panel.constrain(10, board::tft::width() - 100, 10, board::tft::height());
    panel.layout(50, 60);
    panel.render();

    for (;;)
    {
        static int8_t i = 0;

        box1 = i++;
        sys_tick::delay_ms(10);
    }
}

