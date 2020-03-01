#pragma once

#include "clock.h"
#include "euclidean.h"

struct sequence_t
{
    void setup(uint8_t k, uint8_t n)
    {
        m_n = n;
        m_k = k;
        m_step = m_count = m_rate = m_skew = 0;
        update();
    }

    bool fire(uint32_t i)
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

