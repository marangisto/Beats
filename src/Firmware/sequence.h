#pragma once

#include "clock.h"
#include "euclidean.h"
#include <widget.h>

struct isequence
{
    virtual bool fire(uint32_t i) = 0;
};

template<typename DISPLAY>
struct sequence_t: window_t<DISPLAY>, isequence
{
    sequence_t(const theme_t& t)
        : m_k(t, sizeof(m_k)), m_n(t, sizeof(m_fields)), m_rot(t, sizeof(m_frame)), m_dir(t, sizeof(rect_t))
        , m_rate(t, 0), m_skew(t, 0), m_gate(t, 0)
        , m_count(0), m_step(0)
        , m_fields(&m_k, &m_n, &m_rot, &m_dir, &m_rate, &m_skew, &m_gate)
        , m_frame(&m_fields, t.border_color)
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
        update();
    }

    virtual action_t handle_message(const message_t& m)
    {
        if (m.index() == button_press && std::get<button_press>(m) < 8)
            return action_t().emplace<pop_window>(0);
        else
            return window_t<DISPLAY>::handle_message(m);
    }

    // isequence

    virtual bool fire(uint32_t i)
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
            if ((i & clock::clock_mask) == 0 && --m_count <= m_rate)
            {
                m_count = 0;
                return true;
            }
            else
                return false;
        else                        // power of 2 multiplier
            return (i & (clock::clock_mask >> m_rate)) == 0;
    }

    typedef valuebox_t<DISPLAY, show_str> label;
    typedef valuebox_t<DISPLAY, show_int, edit_int> intbox;

    // sequence parameters
 
    intbox              m_k;        // nominal beat count
    intbox              m_n;        // sequence length
    intbox              m_rot;      // sequence rotation
    intbox              m_dir;      // sequence direction
    intbox              m_rate;     // clock rate factor
    intbox              m_skew;     // time shift in ticks
    intbox              m_gate;     // gate channel selector

    // internal state variables

    volatile uint64_t   m_bits;     // realized bit pattern
    volatile int8_t     m_count;    // tick count for time base
    volatile uint8_t    m_step;     // current position in sequence

    // passive gui elements

    vertical_t<DISPLAY> m_fields;
    border_t<DISPLAY>   m_frame;
};

