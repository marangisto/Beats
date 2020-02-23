#pragma once

#include <timer.h>
#include <widget.h>
#include "color.h"

namespace clock
{

typedef hal::timer::timer_t<2> master;
typedef hal::timer::timer_t<14> extra;

static const uint16_t clock_multiplier = 32;                // must be power of 2!
static const uint32_t clock_mask = clock_multiplier - 1;

static uint32_t bpm_arr(float bpm)
{
    float f1 = hal::sys_clock::freq() >> 1;
    float f2 = clock_multiplier * bpm / 60.0;

    return static_cast<uint32_t>(f1 / f2) - 1;
}

enum run_state_t { stopped, running };
enum clock_source_t { internal, external };

static volatile run_state_t run_state = stopped;
static volatile clock_source_t clock_source = internal;

static void master_tick(uint32_t i)
{
    board::out1::set();
    hal::sys_tick::delay_us(25);
    board::out1::clear();

    if (!i)
        board::ledA::set_ms(run_state == running ? 50 : 3);
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
        m_mode.setup(font(), text_fg, text_bg, "intern", &m_quiet);
        m_row.setup();
        m_row.append(&m_bpm);
        m_row.append(&m_mode);
        m_panel.setup(&m_row, frame_fg);
        m_panel.constrain(10, board::tft::width(), 10, board::tft::height());
        m_panel.layout(0, 0);
        m_panel.render();
        m_quiet = false;

        // timer setup

        master::setup(1, bpm_arr(m_bpm));
        master::update_interrupt_enable();
        hal::nvic<interrupt::TIM2>::enable();

        extra::setup(480-1, 65535); // counting up at 100kHz
    }

    void render()
    {
        m_panel.render();
    }

    void handle_message(const message_t& m)
    {
        switch (m.index())
        {
        case button_press:
            switch (std::get<button_press>(m))
            {
            case 10:    // btn A
                run_state = run_state == stopped ? running : stopped;
                break;
            case 100 + internal:
                m_mode = "intern";
                break;
            case 100 + external:
                m_mode = "extern";
                break;
            default: ;  // unhandler button
            }
            break;
        case encoder_delta:
            m_bpm.edit(std::get<encoder_delta>(m));
            master::set_auto_reload_value(bpm_arr(m_bpm));
            break;
        default: ;      // unhandled message
        }
    }

    valuebox_t<DISPLAY, show_bpm, edit_bpm>     m_bpm;
    valuebox_t<DISPLAY, show_str>               m_mode;
    horizontal_t<DISPLAY>                       m_row;
    border_t<DISPLAY>                           m_panel;
    bool                                        m_quiet;
};

} // namespace clock

static volatile uint32_t int_clock_count = 0;
static volatile uint32_t ext_clock_count = 0;

template<> void handler<interrupt::TIM2>()
{
    clock::master::clear_uif();
    if (clock::clock_source == clock::internal)
        clock::master_tick(int_clock_count & clock::clock_mask);
    ++int_clock_count;

    static uint32_t int_last_count = 0;
    static uint32_t ext_last_count = 0;

    if (int_clock_count - int_last_count > 100)
    {
        if (ext_clock_count - ext_last_count == 0)  // see no external trigger
        {
            clock::clock_source = clock::internal;  // switch to internal clock
            board::mq::put(message_t().emplace<button_press>(100 + clock::internal));
        }
        else
        {
            clock::clock_source = clock::external;  // switch to external clock
            board::mq::put(message_t().emplace<button_press>(100 + clock::external));
        }

        int_last_count = int_clock_count;
        ext_last_count = ext_clock_count;
    }
}

void clock_trigger()            // EXTI for external clock
{
    if (clock::clock_source == clock::external)
        clock::master_tick(ext_clock_count & clock::clock_mask);
    ++ext_clock_count;
}

void reset_trigger()            // EXTI for external reset
{
    board::ledC::set_ms(100);
}

