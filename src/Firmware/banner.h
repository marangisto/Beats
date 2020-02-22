#pragma once

#include <widget.h>

namespace banner
{

template<typename DISPLAY>
struct banner_t
{
    void setup()
    {
        const fontlib::font_t& font = fontlib::cmunvt_28;

        m_label.setup(font, color::black, color::yellow, "Beats");
        m_frame.setup(&m_label, color::blue);
    }

    void render()
    {
        m_frame.render();
    }

    valuebox_t<DISPLAY, show_str>   m_label;
    border_t<DISPLAY>               m_frame;
};

} // namespace banner

