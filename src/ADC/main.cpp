#include "../Firmware/board.h"
#include <widget.h>

using hal::sys_tick;
using namespace color;

void clock_trigger() { board::led5::toggle(); }
void reset_trigger() { board::led5::toggle(); }

int main()
{
    board::setup();

    static theme_t t = { white, slate_gray, dim_gray, yellow, orange_red, fontlib::cmunvt_28 };
    static bool q = false;
    static valuebox_t<board::tft, show_int> box0(t, 0, &q), box1(t, 0, &q), box2(t, 0, &q);
    static valuebox_t<board::tft, show_int> box3(t, 0, &q), box4(t, 0, &q);
    static vertical_t<board::tft> col(&box0, &box1, &box2, &box3, &box4);
    static border_t<board::tft> panel(&col, t.border_color);

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

