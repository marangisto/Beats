#include <gpio.h>

using hal::sys_tick;
using namespace hal::gpio;

typedef output_t<PB8> led_a;
typedef output_t<PB9> led_b;
typedef output_t<PA3> trig_1;

void loop();

int main()
{
    led_a::setup();
    led_b::setup();
    trig_1::setup();

    for (;;)
        loop();
}

void loop()
{
    led_a::toggle();

    if (led_a::read())
        led_b::toggle();

    sys_tick::delay_ms(250);

    trig_1::set();
    sys_tick::delay_us(100);
    trig_1::clear();
}

