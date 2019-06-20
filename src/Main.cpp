#include <stdlib.h>
#include <cstdio>
#include <timer.h>
#include <text.h>
#include <draw.h>
#include "Euclidean.h"
#include "List.h"
#include "Board.h"

using namespace hal;
using namespace timer;
using namespace gpio;
using namespace st7789;
using namespace fontlib;

typedef timer::timer_t<6> aux;
typedef timer::timer_t<2> clock;

class sequence_t
{
public:
    void setup(unsigned k, unsigned n)
    {
        m_k = k;
        m_n = n;
        m_step = 0;
        m_last_step = 0;
        update();
    }

    void set_k(unsigned k)
    {
        m_k = std::min(k, 64u);
        update();
    }

    void set_n(unsigned n)
    {
        m_n = std::max(std::min(n, 64u), 1u);
        if (m_step >= m_n)
            m_step = m_n - 1;
        update();
    }

    bool beat()
    {
        bool b = beat(m_step);

        if (++m_step == m_n)
            m_step = 0;
        return b;
    }

    unsigned k() const { return m_k; }
    unsigned n() const { return m_n; }

    bool beat(unsigned i) const
    {
        return (m_bits & (static_cast<uint64_t>(1) << i)) != 0;
    }

    unsigned step() const { return m_step; }
    unsigned last_step() const { return m_last_step; }
    void set_last_step(unsigned s) { m_last_step = s; }

private:
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

    volatile unsigned m_k;
    volatile unsigned m_n;
    volatile unsigned m_step;
    volatile uint64_t m_bits;
    unsigned m_last_step;       // only used by UI
};

static const uint8_t n_chan = 8;

static sequence_t chan[n_chan];

extern "C" void ISR_TIM2(void)
{
    static bool tick = false;

    clock::clear_uif();
    tick = !tick;
    out0::write(led0::write(tick ? chan[0].beat() : false));
    out1::write(led1::write(tick ? chan[1].beat() : false));
    out2::write(led2::write(tick ? chan[2].beat() : false));
    out3::write(led3::write(tick ? chan[3].beat() : false));
    out4::write(led4::write(tick ? chan[4].beat() : false));
    out5::write(led5::write(tick ? chan[5].beat() : false));
    out6::write(led6::write(tick ? chan[6].beat() : false));
    out7::write(led7::write(tick ? chan[7].beat() : false));
}

static bool read_chan_btn(uint8_t& ch)
{
    if (btn0::read()) { ch = 0; return true; }
    if (btn1::read()) { ch = 1; return true; }
    if (btn2::read()) { ch = 2; return true; }
    if (btn3::read()) { ch = 3; return true; }
    if (btn4::read()) { ch = 4; return true; }
    if (btn5::read()) { ch = 5; return true; }
    if (btn6::read()) { ch = 6; return true; }
    if (btn7::read()) { ch = 7; return true; }
    return false;
}

static unsigned xm(unsigned n, unsigned i)
{
    unsigned x0 = i * 239 / n;
    unsigned x1 = (i + 1) * 239 / n;

    return (x0 + x1) >> 1;
}

static unsigned chan_y_pos(unsigned i)
{
    static const unsigned y0 = 40;
    static const unsigned h = (display::height() - y0) / n_chan;
    return y0 + h * i;
}

static void draw_pointer(pen_t<display>& pen)
{
    int h = 10;

    pen.rel_line_to(h >> 1, h);
    pen.rel_line_to(-h, 0);
    pen.rel_line_to(h >> 1, -h);
}

static void plot_sequence(const sequence_t& ch, unsigned y)
{
    pen_t<display> pen(color::yellow);

    pen.move_to(0, y);
    pen.rel_line_to(display::width() - 1, 0);

    const unsigned n = ch.n();

    for (unsigned i = 0; i < n; ++i)
    {
        pen.set_color(ch.beat(i) ? color::yellow : color::black);
        pen.move_to(xm(n, i), y);
        pen.rel_line_to(0, -10);
    }
}


static void update_status(text_renderer_t<display>& tr, uint8_t ch)
{
    char buf[64];

    sprintf(buf, "%d: E(%d,%d)     ", ch, chan[ch].k(), chan[ch].n());
    tr.set_pos(0, tr.line_spacing());
    tr.write(buf);
}

static void update_cursor(sequence_t& ch, unsigned y)
{
    unsigned step = ch.step();
    unsigned last_step = ch.last_step();

    if (step != last_step)
    {
        pen_t<display> pen(color::black);
        unsigned n = ch.n();

        pen.move_to(xm(n, last_step), y);
        draw_pointer(pen);
        pen.set_color(color::red);
        pen.move_to(xm(n, step), y);
        draw_pointer(pen);
        ch.set_last_step(step);
    }
}

void loop(text_renderer_t<display>& tr);

int main()
{
    for (uint8_t i = 0; i < n_chan; ++i)
        chan[i].setup(i + 1, 16);

    initialize_board();

    clock::setup(100, 65535);
    clock::update_interrupt_enable();

    const color_t fg = color::white;
    const color_t bg = color::black;

    display::setup();
    display::clear(bg);

    font_t ft = fontlib::cmunss_24;

    text_renderer_t<display> tr(ft, fg, bg, true);

    for (uint8_t i = 0; i < n_chan; ++i)
        plot_sequence(chan[i], chan_y_pos(i));

    for (;;)
        loop(tr);
}

void loop(text_renderer_t<display>& tr)
{
    static bool edit_steps = false;
    static uint32_t last_action = 0;

    if (btnC::read())
    {
        ledC::write(edit_steps = !edit_steps);
        last_action = sys_tick::count();
    }

    static uint8_t selected_chan = 0;
    static unsigned last_count = -1;

    if (read_chan_btn(selected_chan))
    {
        last_count = edit_steps ? chan[selected_chan].n() : chan[selected_chan].k();
        encoder::set_count(last_count << 1);
        update_status(tr, selected_chan);
        last_action = sys_tick::count();
    }

    unsigned count = encoder::count() >> 1;

    if (count != last_count)
    {
        edit_steps ? chan[selected_chan].set_n(count) : chan[selected_chan].set_k(count);
        plot_sequence(chan[selected_chan], chan_y_pos(selected_chan));
        last_count = count;
        update_status(tr, selected_chan);
        last_action = sys_tick::count();
    }

    for (uint8_t i = 0; i < n_chan; ++i)
        update_cursor(chan[i], chan_y_pos(i));

    if (edit_steps && sys_tick::count() - last_action > 5000)
        ledC::write(edit_steps = false);
}

