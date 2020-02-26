#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"

static const uint16_t trigger_pulse_length = 5;
static const uint16_t trigger_led_length = 75;

void clock_tick(uint32_t i)
{
    if ((i & clock::clock_mask) == (clock::clock_multiplier >> 1))   // pulse on centre-slot
    {
        board::led0::pulse(trigger_led_length);
        board::out0::pulse(trigger_pulse_length);
    }
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

        hal::sys_tick::delay_ms(20);
    }
}

