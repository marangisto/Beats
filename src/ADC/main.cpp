#include "../Firmware/board.h"
#include <widget.h>
#include <adc.h>
#include <dma.h>
#include <timer.h>

using hal::sys_tick;
using hal::sys_clock;
using namespace hal::adc;
using namespace hal::dma;
using namespace hal::timer;

typedef hal::adc::adc_t<1> adc;
typedef hal::dma::dma_t<1> adc_dma;
typedef hal::timer::timer_t<15> adc_tim;

static const uint8_t adc_dma_ch = 1;
static const uint8_t adc_buf_size = 4;
static uint16_t adc_buf[adc_buf_size];
static const uint32_t adc_sample_freq = 2000;

void clock_trigger() {}
void reset_trigger() {}

int main()
{
    board::setup();

    const fontlib::font_t& font = fontlib::cmunvt_28;
    static valuebox_t<board::tft, show_int> box0, box1, box2, box3, box4;
    static vertical_t<board::tft> col;
    static border_t<board::tft> panel;
    static color::color_t bg = color::steel_blue;
    static color::color_t fg = color::yellow;
    static bool quiet = false;

    box0.setup(font, fg, bg, 0, &quiet);
    box1.setup(font, fg, bg, 0, &quiet);
    box2.setup(font, fg, bg, 0, &quiet);
    box3.setup(font, fg, bg, 0, &quiet);
    box4.setup(font, fg, bg, 0, &quiet);
    col.setup();
    col.append(&box0);
    col.append(&box1);
    col.append(&box2);
    col.append(&box3);
    col.append(&box4);
    panel.setup(&col, color::black);
    panel.constrain(10, board::tft::width() - 100, 10, board::tft::height());
    panel.layout(50, 60);
    panel.render();

    adc_tim::setup(1, (sys_clock::freq() >> 1) / adc_sample_freq - 1);
    adc_tim::master_mode<adc_tim::mm_update>();

    adc_dma::setup();

    adc::setup();
    adc::sequence<8, 9, 14, 15>();
    adc::dma<adc_dma, adc_dma_ch>(adc_buf, adc_buf_size);
    adc::trigger<0x4>();    // FIXME: use symbol for TIM15_TRIGO
    adc::enable();
    adc::start_conversion();

    for (;;)
    {
        static int8_t i = 0;

        box0 = i++;
        box1 = adc_buf[2];
        box2 = adc_buf[1];
        box3 = adc_buf[3];
        box4 = adc_buf[0];
        sys_tick::delay_ms(10);
    }
}

