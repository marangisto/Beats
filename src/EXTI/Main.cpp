#include <gpio.h>

typedef input_t<PB7> clk;       // clock input
typedef input_t<PD2> rst;       // reset input

typedef output_t<PA3>  out0;    // channel 0 out
typedef output_t<PF5>  out2;    // channel 2 out
typedef output_t<PA5>  out4;    // channel 4 out

template<> void handler<interrupt::EXTI2_3>()
{
    if (rst::interrupt_pending())
    {
        out4::toggle();
        rst::clear_interrupt();
    }
}

template<> void handler<interrupt::EXTI4_15>()
{
    out0::toggle();

    if (clk::interrupt_pending())
    {
        out2::toggle();
        clk::clear_interrupt();
    }
}

int main()
{
    out0::setup();
    out2::setup();
    out4::setup();

    clk::setup();
    rst::setup();

    clk::enable_interrupt<falling_edge>();
    rst::enable_interrupt<falling_edge>();
    interrupt::set<interrupt::EXTI2_3>();
    interrupt::set<interrupt::EXTI4_15>();

    for (;;) ;
}

