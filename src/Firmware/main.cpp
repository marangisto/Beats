#include "board.h"
#include "banner.h"

using hal::sys_tick;

void clock_trigger()
{
    board::ledA::set_ms(25);
}

void reset_trigger()
{
    board::ledB::set_ms(100);
}

int main()
{
    static banner::banner_t<board::tft> banner;

    board::setup();
    banner.setup();
    banner.render();

    board::clk::enable_interrupt<hal::gpio::falling_edge>();
    board::rst::enable_interrupt<hal::gpio::falling_edge>();
    hal::nvic<interrupt::EXTI4_15>::enable();

    for (;;)
    {
        board::led0::set_ms(25);
        sys_tick::delay_ms(500);
    }
}

