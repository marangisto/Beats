#pragma once

#include <widget.h>
#include "color.h"

#define xstr(s) str(s)
#define str(s) #s

namespace banner
{

template<typename DISPLAY>
struct banner_t
{
    void show()
    {
        m_line1.setup(font(), text_fg, text_bg, "Beats");
        m_line2.setup(font(), text_fg, text_bg, xstr(VERSION));
        m_line3.setup(font(), text_fg, text_bg, "Marangisto");
        m_line4.setup(font(), text_fg, text_bg, "2019-2020");
        m_column.setup();
        m_column.append(&m_line1);
        m_column.append(&m_line2);
        m_column.append(&m_line3);
        m_column.append(&m_line4);
        m_frame.setup(&m_column, frame_fg);
        m_frame.constrain(10, board::tft::width() - 100, 10, board::tft::height());
        m_frame.layout(50, 60);
        DISPLAY::clear(screen_bg);
        m_frame.render();
        hal::sys_tick::delay_ms(1000);
        DISPLAY::clear(screen_bg);
    }

    valuebox_t<DISPLAY, show_str>   m_line1;
    valuebox_t<DISPLAY, show_str>   m_line2;
    valuebox_t<DISPLAY, show_str>   m_line3;
    valuebox_t<DISPLAY, show_str>   m_line4;
    vertical_t<DISPLAY>             m_column;
    border_t<DISPLAY>               m_frame;
};

} // namespace banner

