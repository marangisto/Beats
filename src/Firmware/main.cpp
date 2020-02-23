#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"

using hal::sys_tick;

void clock_trigger()
{
    board::ledA::set_ms(25);
    board::led0::set_ms(1);
}

void reset_trigger()
{
    board::ledB::set_ms(100);
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

