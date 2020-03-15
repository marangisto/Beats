#pragma once

#include <timer.h>
#include <widget.h>

namespace clock
{

using hal::sys_tick;
using hal::sys_clock;

typedef hal::timer::timer_t<2> master;
typedef hal::timer::timer_t<14> timer;
typedef hal::timer::timer_t<16> monitor;

static const uint16_t clock_multiplier = 32;                // must be power of 2!
static const uint32_t clock_mask = clock_multiplier - 1;
static const uint32_t timer_freq = 100000;                  // to measure external bpm

static uint32_t bpm_arr(float bpm)
{
    float f1 = sys_clock::freq() >> 1;
    float f2 = clock_multiplier * bpm / 60.0;

    return static_cast<uint32_t>(f1 / f2) - 1;
}

enum run_state_t { stopped, running };
enum clock_source_t { internal, external };

static volatile run_state_t run_state = stopped;
static volatile clock_source_t clock_source = internal;
static volatile float int_bpm = 0.0f, ext_bpm = 0.0f;

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
struct gui_t: window_t<DISPLAY>
{
    gui_t(const theme_t& t)
        : m_bpm(t, 120.0)
        , m_mode(t, "intern")
        , m_row(&m_bpm, &m_mode)
        , m_panel(&m_row, t.border_color)
    {
        list<ifocus*> navigation;

        navigation.push_back(&m_bpm);
        window_t<DISPLAY>::setup(&m_panel, navigation, t);
        int_bpm = m_bpm;

        // master clock timer setup

        master::setup(1, bpm_arr(m_bpm));
        master::update_interrupt_enable();
        hal::nvic<interrupt::TIM2>::enable();

        // bpm measurement timer setup

        timer::setup(sys_clock::freq() / timer_freq - 1, 65535);    // keep counting

        // monitor timer setup

        monitor::setup(sys_clock::freq() / 100000 - 1, 50000 - 1);  // 2 Hz
        monitor::update_interrupt_enable();
        hal::nvic<interrupt::TIM16>::enable();

        // external clock and reset interrupts

        board::clk::enable_interrupt<hal::gpio::falling_edge>();
        hal::nvic<interrupt::EXTI4_15>::enable();

        board::rst::enable_interrupt<hal::gpio::falling_edge>();
        hal::nvic<interrupt::EXTI2_3>::enable();
    }

    virtual action_t handle_message(const message_t& m)
    {
        if (m.index() == button_press)
            switch (std::get<button_press>(m))
            {
            case 10:    // btn A
                run_state = run_state == stopped ? running : stopped;
                break;
            case 100:
                if (clock_source == internal)
                {
                    m_mode = "intern";
                    m_bpm = static_cast<float>(int_bpm);
                }
                else
                    m_mode = "extern";
                break;
            case 200:
                m_bpm = static_cast<float>(ext_bpm);
                break;
            default: ;  // unhandler button
            }

        if (clock_source == internal)
        {
            action_t a = window_t<DISPLAY>::handle_message(m);
            master::set_auto_reload_value(bpm_arr(int_bpm = m_bpm));
            return a;
        }
        else
            return action_t().emplace<no_action>(unit);
    }

    valuebox_t<DISPLAY, show_bpm, edit_bpm>     m_bpm;
    valuebox_t<DISPLAY, show_str>               m_mode;
    horizontal_t<DISPLAY>                       m_row;
    border_t<DISPLAY>                           m_panel;
};

} // namespace clock

static volatile uint32_t ext_clock_count = 0;

static void tick()
{
    static volatile uint32_t tick_count = 0;
    using namespace clock;

    void clock_tick(uint32_t i);

    if (run_state == running)
        clock_tick(tick_count);
    if ((tick_count & clock_mask) == 0)
        board::ledA::pulse(run_state == running ? 10 : 3);
    ++tick_count;
}

template<> void handler<interrupt::TIM2>()  // master clock
{
    clock::master::clear_uif();

    if (clock::clock_source == clock::internal)
        tick();
}

template<> void handler<interrupt::TIM16>()     // monitor timer
{
    clock::monitor::clear_uif();

    static uint32_t ext_last_count = 0;

    if (ext_clock_count - ext_last_count == 0)  // see no external trigger
    {
        if (clock::clock_source != clock::internal)
        {
            clock::clock_source = clock::internal;  // switch to internal clock
            board::mq::put(message_t().emplace<button_press>(100));
        }
    }
    else
    {
        if (clock::clock_source != clock::external)
        {
            clock::clock_source = clock::external;  // switch to external clock
            board::mq::put(message_t().emplace<button_press>(100));
        }
    }

    ext_last_count = ext_clock_count;
}

template<> void handler<interrupt::EXTI2_3>()   // external reset
{
    if (board::rst::interrupt_pending())
    {
        board::rst::clear_interrupt();
        board::ledC::pulse(100);
    }
}

template<> void handler<interrupt::EXTI4_15>()  // external clock
{
    if (board::clk::interrupt_pending())
    {
        board::clk::clear_interrupt();

        if (clock::clock_source == clock::external)
        {
            static const float k = clock::timer_freq * 60.0f / clock::clock_multiplier;
            static const uint32_t update_cycle = clock::timer_freq / 4;
            static uint32_t elapsed = 0;
            uint16_t n = clock::timer::count();

            clock::timer::set_count(0);
            elapsed += n;

            if (elapsed > update_cycle)
            {
                clock::ext_bpm = n > 0 ? k / n : 0;
                board::mq::put(message_t().emplace<button_press>(200));
                elapsed = 0;
            }

            tick();
        }

        ++ext_clock_count;
    }
}

