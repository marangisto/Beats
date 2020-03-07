#pragma once

#include <widget.h>

#define xstr(s) str(s)
#define str(s) #s

template<typename DISPLAY>
struct banner_t: window_t<DISPLAY>
{
    banner_t(const theme_t& t)
        : m_line1(t, "Beats")
        , m_line2(t, xstr(VERSION))
        , m_line3(t, "Marangisto")
        , m_line4(t, "2019-2020")
        , m_column(&m_line1, &m_line2, &m_line3, &m_line4)
        , m_frame(&m_column, t.border_color, 1)
    {
        list<ifocus*> navigation;
        window_t<DISPLAY>::setup(&m_frame, navigation, t, rect_t(50, 50, 140, 140));
        m_bg = t.normal_bg;
    }

    void show()
    {
        DISPLAY::clear(m_bg);
        window_t<DISPLAY>::render();
        hal::sys_tick::delay_ms(1000);
        DISPLAY::clear(m_bg);
    }

    valuebox_t<DISPLAY, show_str>   m_line1;
    valuebox_t<DISPLAY, show_str>   m_line2;
    valuebox_t<DISPLAY, show_str>   m_line3;
    valuebox_t<DISPLAY, show_str>   m_line4;
    vertical_t<DISPLAY>             m_column;
    border_t<DISPLAY>               m_frame;
    color::color_t                  m_bg;
};

