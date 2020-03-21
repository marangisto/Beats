#include <message.h>
#include "board.h"
#include "banner.h"
#include "sequence.h"

static const uint16_t trigger_pulse_length = 5;
static const uint16_t trigger_led_length = 10;

static sequence_t *chan[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const uint8_t nchan = sizeof(chan) / sizeof(*chan);
static volatile bool update_graphic = false;

template<int CH, typename LED, typename OUT>
void clock_tick(uint32_t i, uint32_t& bits)
{
    if (chan[CH] && chan[CH]->fire(i))
    {
        LED::pulse(trigger_led_length);
        OUT::pulse(trigger_pulse_length);
        bits |= 1 << CH;
    }
}

void clock_tick(uint32_t i)
{
    uint32_t bits = 0;

    clock_tick<0, board::led0, board::out0>(i, bits);
    clock_tick<1, board::led1, board::out1>(i, bits);
    clock_tick<2, board::led2, board::out2>(i, bits);
    clock_tick<3, board::led3, board::out3>(i, bits);
    clock_tick<4, board::led4, board::out4>(i, bits);
    clock_tick<5, board::led5, board::out5>(i, bits);
    clock_tick<6, board::led6, board::out6>(i, bits);
    clock_tick<7, board::led7, board::out7>(i, bits);

    if (update_graphic)
        board::mq::put(message_t().emplace<aux_data>(bits));
}

template<typename DISPLAY>
struct gui_t: iwindow
{
    gui_t(const theme_t& theme)
        : m_filler(filler_t<DISPLAY>::horizontal, color::grey(28), 2)
        , m_canvas(color::black)
        , m_panel(&m_filler, &m_canvas)
        , m_clock(theme, &m_panel)
        , m_sequence(theme)
    {
        m_bw = 28;          // sorry, magic number that 'work'
        m_x0 = 1;
        m_dx = m_bw + 2;
    }

    virtual void render()
    {
        m_clock.render();
        update_graphic = true;
    }

    virtual action_t handle_message(const message_t& m)
    {
        uint8_t i;

        switch (m.index())
        {
        case aux_data:
            {
                using namespace color;

                static const color::color_t fg[nchan] =
                    { black, peru, red, orange
                    , yellow, green, blue, violet
                    };

                static const color::color_t bg[nchan] =
                    { grey(128), grey(118), grey(108), grey(98)
                    , grey(88), grey(78), grey(68), grey(58)
                    };

                pixel_t x = m_x0;
                pixel_t y = m_canvas.scroll(-1);
                uint32_t bits = std::get<aux_data>(m);
                graphics::pen_t<DISPLAY> pen(fg[0]);

                for (unsigned i = 0; i < nchan; ++i, x += m_dx)
                {
                    pen.move_to(x, y);
                    pen.set_color((bits & (1 << i)) ? fg[i] : bg[i]);
                    pen.rel_line_to(m_bw, 0);
                }
            }
            return action_t().emplace<no_action>(unit);
        case button_press:
            if ((i = std::get<button_press>(m)) < nchan)
            {
                update_graphic = false;
                m_canvas.scroll_off();
                m_sequence.bind(i, chan[i]);
                return action_t().emplace<push_window>(&m_sequence);
            }
            // fall throguh to default
        default:
            return m_clock.handle_message(m);
        }
    }

    filler_t<DISPLAY>        m_filler;
    scroll_region_t<DISPLAY> m_canvas;
    vertical_t<DISPLAY>      m_panel;
    clock::gui_t<DISPLAY>    m_clock;
    sequence_gui_t<DISPLAY>  m_sequence;
    pixel_t                  m_x0, m_dx, m_bw;
};

int main()
{
    board::setup();

    using namespace color;

    static theme_t theme = { white, grey(48), black, yellow, orange_red, fontlib::cmunss_24, false };
    static banner_t<board::tft> splash(theme);
    static sequence_t seq[nchan];
    static gui_t<board::tft> gui(theme);

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

