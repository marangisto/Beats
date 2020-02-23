#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"

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

        hal::sys_tick::delay_ms(20);
    }
}

