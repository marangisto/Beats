#include "board.h"
#include "banner.h"

using hal::sys_tick;

int main()
{
    static banner::banner_t<board::tft> banner;

    board::setup();
    banner.setup();
    banner.render();

    for (;;)
    {
        board::led0::toggle();
        sys_tick::delay_ms(250);
    }
}

