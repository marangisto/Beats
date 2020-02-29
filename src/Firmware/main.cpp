#include <message.h>
#include "board.h"
#include "banner.h"
#include "clock.h"
#include "euclidean.h"

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
    void setup(uint8_t k, uint8_t n)
    {
        m_trig.setup();

        m_n = n;
        m_k = k;
        m_step = 0;
        update();
    }

    bool fire(uint32_t i)
    {
        return m_trig.fire(i) && beat();
    }

    bool beat()
    {
        bool b = beat(m_step);

        if (++m_step == m_n)
            m_step = 0;
        return b;
    }

    bool beat(unsigned i) const
    {
        return (m_bits & (static_cast<uint64_t>(1) << i)) != 0;
    }

    void update()
    {
        m_k = std::min(m_k, m_n);
        m_bits = 0;

        if (m_k == 0)
            return;

        auto xs = euclidean(m_k, m_n);
        uint64_t bit = 1;

        for (auto x : xs)
        {
            m_bits |= bit;
            bit = bit << x;
        }
    }

    volatile uint8_t    m_k;
    volatile uint8_t    m_n;
    volatile uint8_t    m_step;
    volatile uint64_t   m_bits;
    trigger_t           m_trig;
};

static channel_t chan[8];
static const uint8_t nchan = sizeof(chan) / sizeof(*chan);

template<int CH, typename LED, typename OUT>
void process(uint32_t i)
{
    if (chan[CH].fire(i))
    {
        LED::pulse(trigger_led_length);
        OUT::pulse(trigger_pulse_length);
    }
}

void clock_tick(uint32_t i)
{
    process<0, board::led0, board::out0>(i);
    process<1, board::led1, board::out1>(i);
    process<2, board::led2, board::out2>(i);
    process<3, board::led3, board::out3>(i);
    process<4, board::led4, board::out4>(i);
    process<5, board::led5, board::out5>(i);
    process<6, board::led6, board::out6>(i);
    process<7, board::led7, board::out7>(i);
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

