#include <stdlib.h>
#include <cstdio>
#include <timer.h>
#include <gpio.h>
#include <button.h>
#include <st7789.h>
#include <text.h>
#include <draw.h>
#include "Euclidean.h"
#include "List.h"

using namespace stm32f0;
using namespace timer;
using namespace gpio;
using namespace st7789;
using namespace fontlib;

typedef timer::timer_t<6> aux;
typedef timer::timer_t<3> clock;

typedef output_t<PC8> led0;
typedef output_t<PC9> led1;

typedef button_t<PA0> encoder_btn;
typedef encoder_t<2, PA15, PB3> encoder;
//typedef encoder_t<3, PB4, PB5> encoder;
typedef st7789_t<1, PA5, PA7, PC5, PC4> display;

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

extern "C" void ISR_TIM3(void)
{
    static bool tick = false;

    clock::clear_uif();
    tick = !tick;
    led0::write(tick ? chan[0].beat() : false);
    led1::write(tick ? chan[1].beat() : false);
    led1::write(tick ? chan[2].beat() : false);
    led1::write(tick ? chan[3].beat() : false);
    led1::write(tick ? chan[4].beat() : false);
    led1::write(tick ? chan[5].beat() : false);
    led1::write(tick ? chan[6].beat() : false);
    led1::write(tick ? chan[7].beat() : false);
}

extern "C" void ISR_TIM6_DAC(void)
{
    aux::clear_uif();
    encoder_btn::update();
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
        chan[i].setup(4, 4 + i);

    led0::setup();
    led1::setup();

    encoder::setup<pull_up>(1 + (64 << 1));
    encoder_btn::setup<pull_down>();

    clock::setup(100, 65535);
    clock::update_interrupt_enable();
    aux::setup(100, 1000);
    aux::update_interrupt_enable();

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
    static unsigned last_count = -1;
    unsigned count = encoder::count() >> 1;

    if (count != last_count)
    {
        char buf[128];

        sprintf(buf, "%05u", count);
        tr.set_pos(0, tr.line_spacing());
        tr.write(buf);

        chan[0].set_k(count);

        plot_sequence(chan[0], chan_y_pos(0));

        last_count = count;
    }

    for (uint8_t i = 0; i < n_chan; ++i)
        update_cursor(chan[i], chan_y_pos(i));
}

