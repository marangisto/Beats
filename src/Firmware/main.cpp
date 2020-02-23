#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"

using hal::sys_tick;

void clock_trigger()
{
    board::led0::set_ms(3);
}

void reset_trigger()
{
    board::ledC::set_ms(100);
}

int main()
{
    static banner::banner_t<board::tft> splash;
    static clock::gui_t<board::tft> gui;

    board::setup();
    splash.show();
    gui.setup();
    gui.render();

    message_t m;

    while (board::mq::get(m)); // discard startup noise

    for (;;)
    {
        if (board::mq::get(m))
            gui.handle_message(m);
    }
}

