#include <gpio.h>

using hal::sys_tick;
using namespace hal::gpio;

typedef input_t<PB7> clk;       // clock input
typedef input_t<PD2> rst;       // reset input

typedef output_t<PA3>  out0;    // channel 0 out
typedef output_t<PF4>  out1;    // channel 1 out
typedef output_t<PF5>  out2;    // channel 2 out
typedef output_t<PA4>  out3;    // channel 3 out
typedef output_t<PA5>  out4;    // channel 4 out
typedef output_t<PB2>  out5;    // channel 5 out
typedef output_t<PB10> out6;    // channel 6 out
typedef output_t<PB11> out7;    // channel 7 out

template<> void handler<interrupt::EXTI4_15>()
{
    bool ci = clk::interrupt_pending();
    bool ri = rst::interrupt_pending();

    out0::toggle();

    if (ci)
        out2::toggle();
    if (ri)
        out4::toggle();
    if (ci)
        clk::clear_interrupt();
    if (ri)
        rst::clear_interrupt();
}

int main()
{
    out0::setup();
    out1::setup();
    out2::setup();
    out3::setup();
    out4::setup();
    out5::setup();
    out6::setup();
    out7::setup();

    clk::setup();
    rst::setup();

    clk::enable_interrupt<falling_edge>();
    rst::enable_interrupt<falling_edge>();
    hal::nvic<interrupt::EXTI4_15>::enable();

    for (;;) ;
}

