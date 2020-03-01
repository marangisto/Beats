#include <message.h>
#include "board.h"
#include "banner.h"
#include "sequence.h"

static const uint16_t trigger_pulse_length = 5;
static const uint16_t trigger_led_length = 10;

static sequence_t chan[8];
static const uint8_t nchan = sizeof(chan) / sizeof(*chan);

template<int CH, typename LED, typename OUT>
void clock_tick(uint32_t i)
{
    if (chan[CH].fire(i))
    {
        LED::pulse(trigger_led_length);
        OUT::pulse(trigger_pulse_length);
    }
}

void clock_tick(uint32_t i)
{
    clock_tick<0, board::led0, board::out0>(i);
    clock_tick<1, board::led1, board::out1>(i);
    clock_tick<2, board::led2, board::out2>(i);
    clock_tick<3, board::led3, board::out3>(i);
    clock_tick<4, board::led4, board::out4>(i);
    clock_tick<5, board::led5, board::out5>(i);
    clock_tick<6, board::led6, board::out6>(i);
    clock_tick<7, board::led7, board::out7>(i);
}

int main()
{
    static banner::banner_t<board::tft> splash;
    static clock::gui_t<board::tft> gui;
    static sequence_ui_t<board::tft> seq_ui;

    board::setup();
    splash.show();

    gui.setup();
    gui.render();

    seq_ui.setup();
    //seq_ui.render();

    for (uint8_t i = 0; i < nchan; ++i)
        chan[i].setup(4, 16);

    chan[0].setup(3, 5);
    chan[1].setup(4, 7);

    message_t m;

    while (board::mq::get(m)); // discard startup noise

    for (;;)
    {
        if (board::mq::get(m))
            gui.handle_message(m);

        hal::sys_tick::delay_ms(20);
    }
}

