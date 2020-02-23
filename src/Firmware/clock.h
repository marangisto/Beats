#pragma once

#include <timer.h>
#include <widget.h>
#include "color.h"

namespace clock
{

struct show_bpm
{
    typedef float T;
    static const char *show(T x) { sprintf(tmp_buf, "%.1f bpm", x); return tmp_buf; }
};

struct edit_bpm
{
    static void edit(volatile float& x, int i)
    {
        if (i > 0 ? (x + i <= 240) : (x + i >= 30))
            x += i;
    }
};


template<typename DISPLAY>
struct gui_t
{
    void setup()
    {
        m_bpm.setup(font(), text_fg, text_bg, 120.0, &m_quiet);
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

    void handle_message(const message_t& m)
    {
        switch (m.index())
        {
        case encoder_delta:
            m_bpm.edit(std::get<encoder_delta>(m));
            break;
        default: ;      // unhandled message
        }
    }

    valuebox_t<DISPLAY, show_bpm, edit_bpm>     m_bpm;
    horizontal_t<DISPLAY>                       m_row;
    border_t<DISPLAY>                           m_panel;
    bool                                        m_quiet;
};

} // namespace clock

