#include "../Firmware/board.h"
#include <widget.h>

using hal::sys_tick;
using namespace color;

void clock_trigger() { board::led5::toggle(); }
void reset_trigger() { board::led5::toggle(); }

int main()
{
    board::setup();
    theme_t theme = { white, slate_gray, dim_gray, yellow, orange_red, fontlib::cmunvt_28 };

    static valuebox_t<board::tft, show_int> box0, box1, box2, box3, box4;
    static vertical_t<board::tft> col;
    static border_t<board::tft> panel;
    static bool quiet = false;

    box0.setup(theme, 0, &quiet);
    box1.setup(theme, 0, &quiet);
    box2.setup(theme, 0, &quiet);
    box3.setup(theme, 0, &quiet);
    box4.setup(theme, 0, &quiet);
    col.setup();
    col.append(&box0);
    col.append(&box1);
    col.append(&box2);
    col.append(&box3);
    col.append(&box4);
    panel.setup(&col, color::black);
    panel.constrain(10, board::tft::width() - 150, 10, board::tft::height());
    panel.place(75, 60);
    panel.render();

    for (;;)
    {
        static int8_t i = 0;

        box0 = i++;
        box1 = board::cvin<0>();
        box2 = board::cvin<1>();
        box3 = board::cvin<2>();
        box4 = board::cvin<3>();
        sys_tick::delay_ms(10);
    }
}

