#include "../Firmware/board.h"
#include <widget.h>

using namespace color;

void clock_trigger() { board::led5::toggle(); }
void reset_trigger() { board::led5::toggle(); }

int main()
{
    board::setup();

    static theme_t t = { white, slate_gray, dim_gray, yellow, orange_red, fontlib::cmunvt_28, false };
    static valuebox_t<board::tft, show_int> box0(t, 0), box1(t, 0), box2(t, 0);
    static valuebox_t<board::tft, show_int> box3(t, 0), box4(t, 0);
    static vertical_t<board::tft> col(&box0, &box1, &box2, &box3, &box4);
    static border_t<board::tft> panel(&col, t.border_color);
    static window_t<board::tft> window;
    list<ifocus*> navigation;

    window.setup(&panel, navigation, t);
    window.render();

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

