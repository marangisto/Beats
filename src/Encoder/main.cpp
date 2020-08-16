#include <gpio.h>
#include <timer.h>

typedef output_t<PC13> led0;     // channel 0 led
typedef output_t<PC15> led1;     // channel 1 led
typedef output_t<PC1>  led2;     // channel 2 led
typedef output_t<PC3>  led3;     // channel 3 led
typedef output_t<PA1>  led4;     // channel 4 led
typedef output_t<PC11> led5;     // channel 5 led
typedef output_t<PA15> led6;     // channel 6 led
typedef output_t<PF7>  led7;     // channel 7 led

typedef output_t<PA8> ledA;      // user led 8
typedef output_t<PC8> ledB;      // user led 9
typedef output_t<PC6> ledC;      // user led 10

using tim = tim_t<14>;
using enc = encoder_t<3, PA6, PA7>;

template<> void handler<interrupt::TIM14>()
{
    tim::clear_update_interrupt_flag();
    ledA::toggle();
}

static void show(uint8_t i)
{
    led0::write(i & (1 << 0));
    led1::write(i & (1 << 1));
    led2::write(i & (1 << 2));
    led3::write(i & (1 << 3));
    led4::write(i & (1 << 4));
    led5::write(i & (1 << 5));
    led6::write(i & (1 << 6));
    led7::write(i & (1 << 7));
}

int main()
{
    led0::setup();
    led1::setup();
    led2::setup();
    led3::setup();
    led4::setup();
    led5::setup();
    led6::setup();
    led7::setup();
    ledA::setup();
    ledB::setup();
    ledC::setup();

    enc::setup<pull_up>(1 + (64 << 1));

    // f_tim = f_sysclock / ((psc + 1) (arr + 1))

    tim::setup(sys_clock::freq() / 10000, 9999);
    tim::enable_update_interrupt();
    interrupt::set<interrupt::TIM14>();
    interrupt::enable();

    for (;;)
        show(enc::count());
}

