#pragma once

#include <draw.h>
#include <color.h>

template<typename DISPLAY>
struct graphic_t: iwidget
{
    graphic_t(const sequence_t *const *chan, unsigned nchan): m_chan(chan), m_nchan(nchan)
    {
        m_tpp = 1;
    }

    // iwidget

    virtual void size(pixel_t& w, bool& fill_h, pixel_t& h, bool& fill_v)
    {
        w = h = 2;  // any small number
        fill_h = fill_v = true;
    }

    virtual void size(pixel_t w, pixel_t h)
    {
        m_rect.w = w;
        m_rect.h = h;
        m_bar.w = 2 * w / (3 * m_nchan + 1);
        m_bar.h = 8 * h / 10;
    }

    virtual void place(pixel_t x, pixel_t y)
    {
        m_rect.x = x;
        m_rect.y = y;
        m_bar.y = y + m_rect.h / 10;
        m_bar.x = m_bar.w / 2;
    }

    virtual void render()
    {
        graphics::pen_t<DISPLAY> pen(color::black);

        pen.fill_rectangle(m_rect.x, m_rect.y, m_rect.w, m_rect.h);

        pixel_t x = 0, dx = m_bar.x + m_bar.w;

        for (unsigned i = 0; i < m_nchan; ++i, x += dx)
        {
            pen.set_color(color::dark_gray);
            pen.fill_rectangle(m_bar.x + x, m_bar.y, m_bar.w, m_bar.h);
            pen.set_color(color::white);

            for (unsigned j = 0; j < m_bar.h; ++j)
            {
                if (m_chan[i]->beat(j))
                {
                    pen.move_to(m_bar.x + x, m_bar.y + j);
                    pen.rel_line_to(m_bar.w, 0);
                }
            }
        }
    }

    const sequence_t    *const *m_chan;
    const unsigned      m_nchan;
    rect_t              m_rect;
    rect_t              m_bar;
    unsigned            m_tpp;  // ticks per pixel
};

