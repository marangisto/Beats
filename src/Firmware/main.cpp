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

struct gui_t: iwindow
{
    gui_t(const theme_t& theme)
        : m_canvas(theme)
        , m_clock(theme, &m_canvas)
        , m_sequence(theme)
        , m_theme(theme)
    {
        rect_t r = m_canvas.rect();

        m_bw = 4 * r.w / (5 * nchan + 1);
        m_x0 = m_bw / 4;
        m_dx = m_bw + m_x0;
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
                static const color::color_t fg[nchan] =
                    { color::black
                    , color::brown
                    , color::red
                    , color::orange
                    , color::yellow
                    , color::green
                    , color::blue
                    , color::violet
                    };

                pixel_t x = m_x0;
                pixel_t y = m_canvas.scroll(-1);
                uint32_t bits = std::get<aux_data>(m);
                graphics::pen_t<board::tft> pen(m_theme.normal_fg);

                for (unsigned i = 0; i < nchan; ++i, x += m_dx)
                {
                    pen.move_to(x, y);
                    pen.set_color((bits & (1 << i)) ? fg[i] : m_theme.normal_bg);
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

    scroll_region_t<board::tft> m_canvas;
    clock::gui_t<board::tft>    m_clock;
    sequence_gui_t<board::tft>  m_sequence;
    const theme_t&              m_theme;
    pixel_t                     m_x0, m_dx, m_bw;
};

int main()
{
    board::setup();

    using namespace color;

    static theme_t theme = { white, slate_gray, black, yellow, orange_red, fontlib::cmunss_24, false };
    static banner_t<board::tft> splash(theme);
    static sequence_t seq[nchan];
    static gui_t gui(theme);

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

