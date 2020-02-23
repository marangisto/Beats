#pragma once

#include <timer.h>
#include <widget.h>
#include "color.h"

namespace clock
{

typedef hal::timer::timer_t<2> tim;

static const uint8_t clock_multiplier = 32;

static uint32_t bpm_arr(float bpm)
{
    float f1 = hal::sys_clock::freq() >> 1;
    float f2 = clock_multiplier * bpm / 60.0;

    return static_cast<uint32_t>(f1 / f2) - 1;
}

struct show_bpm
{
    typedef float T;
    static const char *show(T x) { sprintf(tmp_buf, "%.1f bpm", x); return tmp_buf; }
};

struct edit_bpm
{
    static void edit(volatile float& x, int i)
    {
        if (i > 0 ? (x + i <= 240) : (x + i >= 30))
            x += i;
    }
};

template<typename DISPLAY>
struct gui_t
{
    void setup()
    {
        m_bpm.setup(font(), text_fg, text_bg, 120.0, &m_quiet);
        m_arr.setup(font(), text_fg, text_bg, 0, &m_quiet);
        m_row.setup();
        m_row.append(&m_bpm);
        m_row.append(&m_arr);
        m_panel.setup(&m_row, frame_fg);
        m_panel.constrain(10, board::tft::width() - 100, 10, board::tft::height());
        m_panel.layout(50, 60);
        m_panel.render();
        m_quiet = false;

        // timer setup

        tim::setup(1, bpm_arr(m_bpm));
        tim::update_interrupt_enable();
        hal::nvic<interrupt::TIM2>::enable();
    }

    void render()
    {
        m_panel.render();
    }

    void handle_message(const message_t& m)
    {
        switch (m.index())
        {
        case encoder_delta:
            m_bpm.edit(std::get<encoder_delta>(m));
            m_arr = bpm_arr(m_bpm);
            tim::set_auto_reload_value(m_arr);
            break;
        default: ;      // unhandled message
        }
    }

    valuebox_t<DISPLAY, show_bpm, edit_bpm>     m_bpm;
    valuebox_t<DISPLAY, show_int>               m_arr;
    //horizontal_t<DISPLAY>                       m_row;
    vertical_t<DISPLAY>                       m_row;
    border_t<DISPLAY>                           m_panel;
    bool                                        m_quiet;
};

} // namespace clock

template<> void handler<interrupt::TIM2>()
{
    using namespace board;

    board::out1::toggle();
    clock::tim::clear_uif();
}

