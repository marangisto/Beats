#include <message.h>
#include "board.h"
#include "banner.h"
#include "sequence.h"

static const uint16_t trigger_pulse_length = 5;
static const uint16_t trigger_led_length = 10;

static sequence_t *chan[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const uint8_t nchan = sizeof(chan) / sizeof(*chan);

template<int CH, typename LED, typename OUT>
void clock_tick(uint32_t i)
{
    if (chan[CH] && chan[CH]->fire(i))
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

struct gui_t: iwindow
{
    gui_t(const theme_t& theme, iwindow *ground)
        : m_ground(ground)
        , m_sequence(theme)
    {}

    virtual void render() { m_ground->render(); }

    virtual action_t handle_message(const message_t& m)
    {
        uint8_t i;

        if (m.index() == button_press && (i = std::get<button_press>(m)) < nchan)
        {
            m_sequence.bind(chan[i]);
            return action_t().emplace<push_window>(&m_sequence);
        }
        else
            return m_ground->handle_message(m);
    }

    iwindow                     *m_ground;
    sequence_gui_t<board::tft>  m_sequence;
};

int main()
{
    board::setup();

    using namespace color;

    static theme_t theme = { white, slate_gray, black, yellow, orange_red, fontlib::cmunss_20, false };
    static banner_t<board::tft> splash(theme);
    static clock::gui_t<board::tft> clock(theme);
    static sequence_t seq[nchan];
    static gui_t gui(theme, &clock);

    splash.show();

    for (uint8_t i = 0; i < nchan; ++i)
        chan[i] = &seq[i];

    window_manager wm(&gui);
    message_t m;

    while (board::mq::get(m)); // discard startup noise

    for (;;)
    {
        if (board::mq::get(m))
            wm.handle_message(m);
        hal::sys_tick::delay_ms(1);
    }
}

