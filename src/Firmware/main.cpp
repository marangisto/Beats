#include "board.h"
#include "banner.h"
#include "clock.h"

using hal::sys_tick;

void clock_trigger()
{
    board::ledA::set_ms(25);
}

void reset_trigger()
{
    board::ledB::set_ms(100);
}

int main()
{
    static banner::banner_t<board::tft> banner;
    static clock::gui_t<board::tft> clock;

    board::setup();
    banner.setup();
    banner.render();
    sys_tick::delay_ms(1000);
    board::tft::clear(screen_bg);
    clock.setup();
    clock.render();

    for (;;)
    {
        board::led0::set_ms(25);
        sys_tick::delay_ms(500);
    }
}

