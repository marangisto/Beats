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
struct toolbar_t: horizontal_t<DISPLAY>
{
    toolbar_t(const theme_t& t)
        : horizontal_t<DISPLAY>(&m_boxa, &m_boxb, &m_boxc)
        , m_boxa(t, "start")
        , m_boxb(t, "beats")
        , m_boxc(t, "menu")
    {}

    list<ifocus*> navigation()
    {
        list<ifocus*> navigation;

        //navigation.push_back(&m_boxb);
        return navigation;
    }

    valuebox_t<DISPLAY, show_str>   m_boxa;
    valuebox_t<DISPLAY, show_str>   m_boxb;
    valuebox_t<DISPLAY, show_str>   m_boxc;
};

template<typename DISPLAY>
struct gui_t: window_t<DISPLAY>
{
    gui_t(const theme_t& theme)
        : m_clock(theme, m_toolbar.m_boxa)
        , m_filler(filler_t<DISPLAY>::horizontal, color::grey(18), 3)
        , m_canvas(color::black)
        , m_filler2(filler_t<DISPLAY>::horizontal, color::grey(18), 3)
        , m_toolbar(theme)
        , m_panel(&m_clock, &m_filler, &m_canvas, &m_filler2, &m_toolbar)
        , m_sequence(theme)
    {
        m_bw = 29;          // sorry, magic number that 'work'
        m_x0 = 0;
        m_dx = m_bw + 1;

        list<ifocus*> navigation = m_clock.navigation();

        window_t<DISPLAY>::setup(&m_panel, navigation, theme);
    }

    virtual void render()
    {
        m_panel.render();

        graphics::pen_t<DISPLAY> pen(color::black);
        m_canvas.scroll(-1);
        rect_t r = m_canvas.rect();
        pixel_t x = m_x0;

        for (unsigned i = 0; i < nchan; ++i, x += m_dx)
        {
            pen.set_color(color::grey(128 - i * 10));
            pen.fill_rectangle(r.x + x, r.y, m_bw, r.h);
            pen.set_color(color::black);
            pen.fill_rectangle(r.x + x + m_bw, r.y, m_dx - m_bw, r.h);
        }

        update_graphic = true;
    }

    virtual action_t handle_message(const message_t& m)
    {
        uint8_t i;

        static const color::color_t fg[nchan] =
            { color::black, color::wheat, color::red, color::orange
            , color::yellow, color::green, color::deep_sky_blue, color::violet
            };

        switch (m.index())
        {
        case aux_data:
            {
                pixel_t x = m_x0;
                pixel_t y = m_canvas.scroll(-1);
                uint32_t bits = std::get<aux_data>(m);
                graphics::pen_t<DISPLAY> pen(fg[0]);

                for (unsigned i = 0; i < nchan; ++i, x += m_dx)
                {
                    pen.move_to(x, y);
                    pen.set_color((bits & (1 << i)) ? fg[i] : color::grey(128 - i * 10));
                    pen.rel_line_to(m_bw - 1, 0);
                }
            }
            return action_t().emplace<no_action>(unit);
        case button_press:
            if ((i = std::get<button_press>(m)) < nchan)
            {
                update_graphic = false;
                m_canvas.scroll_off();
                m_sequence.bind(i, chan[i], fg[i]);
                return action_t().emplace<push_window>(&m_sequence);
            }
            // fall throguh to default
        default:
            {
                action_t a = window_t<DISPLAY>::handle_message(m);

                m_clock.handle_message(m);
                return a;
            }
        }
    }

    clock::clock_t<DISPLAY>  m_clock;
    filler_t<DISPLAY>        m_filler;
    scroll_region_t<DISPLAY> m_canvas;
    filler_t<DISPLAY>        m_filler2;
    toolbar_t<DISPLAY>       m_toolbar;
    vertical_t<DISPLAY>      m_panel;
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

