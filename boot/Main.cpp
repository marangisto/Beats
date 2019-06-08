#include <gpio.h>
#include <button.h>
#include <timer.h>

using namespace stm32f0;
using namespace timer;
using namespace gpio;

typedef output_t<PC13> led_0;   // channel 0 led
typedef output_t<PC15> led_1;   // channel 1 led
typedef output_t<PC1>  led_2;   // channel 2 led
typedef output_t<PC3>  led_3;   // channel 3 led
typedef output_t<PA1>  led_4;   // channel 4 led
typedef output_t<PC11> led_5;   // channel 5 led
typedef output_t<PA15> led_6;   // channel 6 led
typedef output_t<PF7>  led_7;   // channel 7 led

typedef output_t<PA8> led_8;    // user led 8 
typedef output_t<PC8> led_9;    // user led 9 
typedef output_t<PC6> led_10;   // user led 10 

typedef output_t<PB8> led_rd;   // rear red led
typedef output_t<PB9> led_yw;   // rear yellow led

typedef button_t<PC14> btn_0;     // user button 0
typedef button_t<PC0>  btn_1;     // user button 1
typedef button_t<PC2>  btn_2;     // user button 2
typedef button_t<PA0>  btn_3;     // user button 3
typedef button_t<PA2>  btn_4;     // user button 4
typedef button_t<PC10> btn_5;     // user button 5
typedef button_t<PA12> btn_6;     // user button 6
typedef button_t<PF6>  btn_7;     // user button 7

typedef button_t<PA11> btn_8;     // user button 8
typedef button_t<PC9>  btn_9;     // user button 9
typedef button_t<PC7>  btn_10;    // user button 10
typedef button_t<PB6>  btn_11;    // encoder button

typedef timer_t<6> aux;

extern "C" void ISR_TIM6_DAC(void)
{
    aux::clear_uif();

    btn_0::update();
    btn_1::update();
    btn_2::update();
    btn_3::update();
    btn_4::update();
    btn_5::update();
    btn_6::update();
    btn_7::update();

    btn_8::update();
    btn_9::update();
    btn_10::update();
    btn_11::update();
}

void loop();

int main()
{
    led_0::setup();
    led_1::setup();
    led_2::setup();
    led_3::setup();
    led_4::setup();
    led_5::setup();
    led_6::setup();
    led_7::setup();
    led_8::setup();
    led_9::setup();
    led_10::setup();

    led_rd::setup();
    led_yw::setup();

    btn_0::setup<pull_up>();
    btn_1::setup<pull_up>();
    btn_2::setup<pull_up>();
    btn_3::setup<pull_up>();
    btn_4::setup<pull_up>();
    btn_5::setup<pull_up>();
    btn_6::setup<pull_up>();
    btn_7::setup<pull_up>();

    btn_8::setup<pull_up>();
    btn_9::setup<pull_up>();
    btn_10::setup<pull_up>();
    btn_11::setup<pull_up>();

    aux::setup(100, 1000);
    aux::update_interrupt_enable();

    for (;;)
        loop();
}

void loop()
{
    led_rd::toggle();

    if (led_rd::read())
        led_yw::toggle();

    if (btn_0::read())
        led_0::toggle();

    if (btn_1::read())
        led_1::toggle();

    if (btn_2::read())
        led_2::toggle();

    if (btn_3::read())
        led_3::toggle();

    if (btn_4::read())
        led_4::toggle();

    if (btn_5::read())
        led_5::toggle();

    if (btn_6::read())
        led_6::toggle();

    if (btn_7::read())
        led_7::toggle();

    if (btn_8::read())
        led_8::toggle();

    if (btn_9::read())
        led_9::toggle();

    if (btn_10::read())
        led_10::toggle();

    if (btn_11::read())
    {
        led_0::toggle();
        led_1::toggle();
        led_2::toggle();
        led_3::toggle();
        led_4::toggle();
        led_5::toggle();
        led_6::toggle();
        led_7::toggle();
    }

    sys_tick::delay_ms(1);
}

