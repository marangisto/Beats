#pragma once

#include <timer.h>
#include <widget.h>
#include "color.h"

namespace clock
{

template<typename DISPLAY>
struct gui_t
{
    void setup()
    {
        m_bpm.setup(font(), text_fg, text_bg, 0, &m_quiet);
        m_row.setup();
        m_row.append(&m_bpm);
        m_panel.setup(&m_row, frame_fg);
        m_panel.constrain(10, board::tft::width() - 100, 10, board::tft::height());
        m_panel.layout(50, 60);
        m_panel.render();
        m_quiet = false;
    }

    void render()
    {
        m_panel.render();
    }

    valuebox_t<DISPLAY, show_int>   m_bpm;
    horizontal_t<DISPLAY>           m_row;
    border_t<DISPLAY>               m_panel;
    bool                            m_quiet;
};

} // namespace clock

