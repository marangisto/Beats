#include <stdlib.h>
#include <cstdio>
#include <timer.h>
#include <gpio.h>
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

typedef timer::timer_t<3> clock;
typedef output_t<PC8> led1;
typedef output_t<PC9> led2;

typedef encoder_t<2, PA15, PB3> encoder;
//typedef encoder_t<3, PB4, PB5> encoder;
typedef st7789_t<1, PA5, PA7, PC5, PC4> display;

class sequence_t
{
public:
    void setup(unsigned k, unsigned n)
    {
        m_n = n;
        m_k = 1;
        m_bits = 0xdeadbeef;
        m_step = 0;
    }

    void set_k(unsigned k)
    {
        m_k = std::min(k, 32u);
        update();
    }

    void set_n(unsigned n)
    {
        m_n = std::max<unsigned>(std::min<unsigned>(n, 32), 1);
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

    unsigned get_n() const { return m_n; }

    bool beat(unsigned i) const { return (m_bits & (1 << i)) != 0; }

private:
    void update()
    {
        m_k = std::min(m_k, m_n);
        m_bits = 0;

        if (m_k == 0)
            return;

        auto xs = euclidean(m_k, m_n);
        unsigned bit = 1;

        for (auto x : xs)
        {
            m_bits |= bit;
            bit = bit << x;
        }
    }

    volatile unsigned m_k;
    volatile unsigned m_n;
    volatile uint32_t m_bits;
    volatile uint8_t m_step;
};

static sequence_t channel0;

extern "C" void ISR_TIM3(void)
{
    clock::clear_uif();
    led2::toggle();
    led1::write(led2::read() ? channel0.beat() : false);
}

void loop(text_renderer_t<display>& tr);

int main()
{
    channel0.setup(0, 32);

    clock::setup(100, 65535);
    clock::update_interrupt_enable();

    led1::setup();
    led2::setup();
    encoder::setup<pull_up>(1 + (16 << 1));
    display::setup();

    font_t ft = fontlib::cmunss_24;
    const color_t fg = color::white;
    const color_t bg = color::black;

    display::clear(bg);
    text_renderer_t<display> tr(ft, fg, bg, true);

    tr.set_pos(50, 50);
    tr.write("Welcome to Beats!");

    for (;;)
        loop(tr);
}

void loop(text_renderer_t<display>& tr)
{
    static uint32_t last_count = -1;
    uint32_t count = encoder::count() >> 1;

    if (count != last_count)
    {
        char buf[128];

        sprintf(buf, "%05ld", count);
        tr.set_pos(50, 100);
        tr.write(buf);

        channel0.set_k(count);

        unsigned n = channel0.get_n();

        pen_t<display> pen(color::yellow);

        pen.move_to(0, 150);
        pen.line_to(239, 150);

        for (unsigned i = 0; i < n; ++i)
        {
            unsigned x0 = i * 239 / n;
            unsigned x1 = (i + 1) * 239 / n;
            unsigned xm = (x0 + x1) >> 1;

            pen.set_color(channel0.beat(i) ? color::yellow : color::black);

            pen.move_to(xm, 140);
            pen.line_to(xm, 160);
        }

        last_count = count;
    }
}

