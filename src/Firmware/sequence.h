#pragma once

#include "clock.h"
#include "euclidean.h"
#include <widget.h>
#include "color.h"

struct isequence
{
    virtual bool fire(uint32_t i) = 0;
};

struct sequence_t: isequence
{
    sequence_t(const theme_t& t)
    {
        m_n = 8;
        m_k = 5;
        m_step = m_count = m_rate = m_skew = 0;
        update();
    }

    virtual bool fire(uint32_t i)
    {
        return tick(i) && beat();
    }

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

    // sequence parameters
 
    volatile uint8_t    m_k;        // nominal beat count
    volatile uint8_t    m_n;        // sequence length
    volatile int8_t     m_rate;     // clock rate factor
    volatile int8_t     m_skew;     // time shift in ticks
 
    // internal state variables

    volatile uint64_t   m_bits;     // realized bit pattern
    volatile int8_t     m_count;    // tick count for time base
    volatile uint8_t    m_step;     // current position in sequence
};

template<typename DISPLAY>
struct sequence_ui_t: public window_t<DISPLAY>
{
    enum state_t { navigating, editing };
    typedef valuebox_t<DISPLAY, show_str> label;
    typedef valuebox_t<DISPLAY, show_int, edit_int> intbox;

    void setup()
    {
        m_panel.setup();

        m_labels.setup();
        l_k.setup(font(), text_fg, text_bg, "beats");
        l_n.setup(font(), text_fg, text_bg, "length");
        l_rotn.setup(font(), text_fg, text_bg, "rotation");
        l_dir.setup(font(), text_fg, text_bg, "direction");
        l_rate.setup(font(), text_fg, text_bg, "rate");
        l_skew.setup(font(), text_fg, text_bg, "skew");
        l_gate.setup(font(), text_fg, text_bg, "gate");
        m_labels.append(&l_k);
        m_labels.append(&l_n);
        m_labels.append(&l_rotn);
        m_labels.append(&l_dir);
        m_labels.append(&l_rate);
        m_labels.append(&l_skew);
        m_labels.append(&l_gate);

        m_controls.setup();
        m_k.setup(font(), text_fg, text_bg, 1);
        m_n.setup(font(), text_fg, text_bg, 4);
        m_rotn.setup(font(), text_fg, text_bg, 0);
        m_dir.setup(font(), text_fg, text_bg, 0);
        m_rate.setup(font(), text_fg, text_bg, 0);
        m_skew.setup(font(), text_fg, text_bg, 0);
        m_gate.setup(font(), text_fg, text_bg, 0);
        m_controls.append(&m_k);
        m_controls.append(&m_n);
        m_controls.append(&m_rotn);
        m_controls.append(&m_dir);
        m_controls.append(&m_rate);
        m_controls.append(&m_skew);
        m_controls.append(&m_gate);

        m_panel.append(&m_labels);
        m_panel.append(&m_controls);

        list<ifocus*> navigation;

        navigation.push_back(&m_k);
        navigation.push_back(&m_n);
        navigation.push_back(&m_rotn);
        navigation.push_back(&m_dir);
        navigation.push_back(&m_rate);
        navigation.push_back(&m_skew);
        navigation.push_back(&m_gate);

        window_t<DISPLAY>::setup(&m_panel, navigation, normal_cursor, active_cursor);
    }

    vertical_t<DISPLAY>     m_labels;
    vertical_t<DISPLAY>     m_controls;
    horizontal_t<DISPLAY>   m_panel;
    label                   l_k;
    label                   l_n;
    label                   l_rotn;
    label                   l_dir;
    label                   l_rate;
    label                   l_skew;
    label                   l_gate;
    intbox                  m_k;
    intbox                  m_n;
    intbox                  m_rotn;
    intbox                  m_dir;
    intbox                  m_rate;
    intbox                  m_skew;
    intbox                  m_gate;
};

