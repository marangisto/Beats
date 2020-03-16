#pragma once

#include "clock.h"
#include "euclidean.h"
#include <widget.h>

struct sequence_t
{
    sequence_t()
        : m_k(4), m_n(8), m_rot(0), m_dir(0)
        , m_rate(0), m_skew(0), m_gate(0)
        , m_count(0), m_step(0)
    {
        update();
    }

    bool fire(uint32_t i)
    {
        return tick(i) && beat();
    }

    void update()
    {
        m_k = std::min<int>(m_k, m_n);
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

    bool beat()
    {
        bool b = beat(m_step);

        if (++m_step == m_n)
            m_step = 0;
        return b;
    }

    bool beat(unsigned step) const  // is there a beat on given step
    {
        return (m_bits & (static_cast<uint64_t>(1) << step)) != 0;
    }

    bool tick(uint32_t i)           // clock tick under rate and skew
    {
        i += m_skew;

        if (m_rate < 0)             // division via counter
            if ((i & clock::clock_mask) == 0 && --m_count < m_rate)
            {
                m_count = 0;
                return true;
            }
            else
                return false;
        else                        // power of 2 multiplier
            return (i & (clock::clock_mask >> m_rate)) == 0;
    }

    // sequence parameters
 
    volatile uint8_t    m_k;        // nominal beat count
    volatile uint8_t    m_n;        // sequence length
    volatile int8_t     m_rot;      // sequence rotation
    volatile uint8_t    m_dir;      // sequence direction
    volatile int8_t     m_rate;     // clock rate factor
    volatile int8_t     m_skew;     // time shift in ticks
    volatile uint8_t    m_gate;     // gate channel selector

    // internal state variables

    volatile uint64_t   m_bits;     // realized bit pattern
    volatile int8_t     m_count;    // tick count for time base
    volatile uint8_t    m_step;     // current position in sequence
};

struct show_dir
{
    typedef int T;
    static const char *show(T x) { sprintf(tmp_buf, "%s", x ? "reverse" : "forward"); return tmp_buf; }
};

struct edit_dir
{
    static void edit(volatile int& x, int i) { if (i) x = !x; }
};

struct show_rate
{
    typedef int T;
    static const char *show(T x)
        {
            if (x == 0)
                sprintf(tmp_buf, "-");
            else if (x < 0)
                sprintf(tmp_buf, " / %d", 1 - x);
            else
                sprintf(tmp_buf, " * %d", 1 << x);
            return tmp_buf;
        }
};

struct edit_rate
{
    static void edit(volatile int& x, int i)
    {
        if (x + i > -99 && x + i < 5) 
            x += i;
    }
};

template<typename DISPLAY>
struct sequence_gui_t: window_t<DISPLAY>
{
    sequence_gui_t(const theme_t& t)
        : m_chan(t, 0), m_k(t, 0), m_n(t, 0), m_rot(t, 0), m_dir(t, 0)
        , m_rate(t, 0), m_skew(t, 0), m_gate(t, 0)
        , m_fields(&m_chan, &m_k, &m_n, &m_rot, &m_dir, &m_rate, &m_skew, &m_gate)
        , l_chan(t, "channel"), l_k(t, "beats"), l_n(t, "steps"), l_rot(t, "rotate")
        , l_dir(t, "order") , l_rate(t, "rate"), l_skew(t, "skew"), l_gate(t, "gate")
        , m_labels(&l_chan, &l_k, &l_n, &l_rot, &l_dir, &l_rate, &l_skew, &l_gate)
        , m_columns(&m_labels, &m_fields)
        , m_frame(&m_columns, t.border_color), m_seq(0)
    {
        list<ifocus*> navigation;

        navigation.push_back(&m_k);
        navigation.push_back(&m_n);
        navigation.push_back(&m_rot);
        navigation.push_back(&m_dir);
        navigation.push_back(&m_rate);
        navigation.push_back(&m_skew);
        navigation.push_back(&m_gate);

        window_t<DISPLAY>::setup(&m_frame, navigation, t, rect_t(50, 50, 140, 140));
    }

    virtual action_t handle_message(const message_t& m)
    {
        if (m.index() == button_press && std::get<button_press>(m) < 8)
            return action_t().emplace<pop_window>(0);
        else
        {
            action_t a = window_t<DISPLAY>::handle_message(m);

            m_k = std::min<uint8_t>(m_k, m_n);

            if (m_seq)
            {
                m_seq->m_k = m_k;
                m_seq->m_n = m_n;
                m_seq->m_rot = m_rot;
                m_seq->m_dir = m_dir;
                m_seq->m_rate = m_rate;
                m_seq->m_skew = m_skew;
                m_seq->m_gate = m_gate;
                m_seq->update();
            }

            return a;
        }
    }

    void bind(int chan, sequence_t *seq)
    {
        m_chan = chan;
        m_seq = seq;
        m_k = m_seq->m_k;
        m_n = m_seq->m_n;
        m_rot = m_seq->m_rot;
        m_dir = m_seq->m_dir;
        m_rate = m_seq->m_rate;
        m_skew = m_seq->m_skew;
        m_gate = m_seq->m_gate;
    }

    typedef valuebox_t<DISPLAY, show_str> label;
    typedef valuebox_t<DISPLAY, show_int, edit_int> intbox;
    typedef valuebox_t<DISPLAY, show_dir, edit_dir> dirbox;
    typedef valuebox_t<DISPLAY, show_rate, edit_rate> ratebox;

    intbox                  m_chan;     // channel number (no navigation)
    intbox                  m_k;        // nominal beat count
    intbox                  m_n;        // sequence length
    intbox                  m_rot;      // sequence rotation
    dirbox                  m_dir;      // sequence direction
    ratebox                 m_rate;     // clock rate factor
    intbox                  m_skew;     // time shift in ticks
    intbox                  m_gate;     // gate channel selector
    vertical_t<DISPLAY>     m_fields;
    label                   l_chan;
    label                   l_k;
    label                   l_n;
    label                   l_rot;
    label                   l_dir;
    label                   l_rate;
    label                   l_skew;
    label                   l_gate;
    vertical_t<DISPLAY>     m_labels;
    horizontal_t<DISPLAY>   m_columns;
    border_t<DISPLAY>       m_frame;
    sequence_t              *m_seq;     // current editing target
};

