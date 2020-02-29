#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"

static const uint16_t trigger_pulse_length = 5;
static const uint16_t trigger_led_length = 10;


struct trigger_t
{
    void setup() { m_rate = m_skew = m_count = 0; }

    bool fire(uint32_t i)
    {
        i += m_skew;

        if (m_rate < 0)     // division via counter
            if ((i & clock::clock_mask) == 0 && --m_count <= m_rate)
            {
                m_count = 0;
                return true;
            }
            else
                return false;
        else                // power of 2 multiplier
            return (i & (clock::clock_mask >> m_rate)) == 0;
    }

    volatile int8_t  m_rate;
    volatile int8_t  m_skew;
    volatile int8_t  m_count;
};

struct channel_t
{
    void setup()
    {
        m_trig.setup();
    }

    bool fire(uint32_t i)
    {
        return m_trig.fire(i);
    }

    trigger_t      m_trig;
};

static channel_t chan[8];
static const uint8_t nchan = sizeof(chan) / sizeof(*chan);

void clock_tick(uint32_t i)
{
    if (chan[0].fire(i))
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

    for (uint8_t i = 0; i < nchan; ++i)
        chan[i].setup();

    chan[0].m_trig.m_rate = 0;

    message_t m;

    while (board::mq::get(m)); // discard startup noise

    for (;;)
    {
        if (board::mq::get(m))
            gui.handle_message(m);

        hal::sys_tick::delay_ms(20);
    }
}

