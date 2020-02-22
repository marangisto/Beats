#include <cstdio>
#include <gpio.h>
#include <button.h>
#include <timer.h>
#include <st7789.h>
#include <draw.h>
#include <text.h>

using hal::sys_tick;
using namespace hal::timer;
using namespace hal::gpio;
using namespace st7789;
using namespace fontlib;
using namespace text;

typedef output_t<PA3>  out_0;   // channel 0 out
typedef output_t<PF4>  out_1;   // channel 1 out
typedef output_t<PF5>  out_2;   // channel 2 out
typedef output_t<PA4>  out_3;   // channel 3 out
typedef output_t<PA5>  out_4;   // channel 4 out
typedef output_t<PB2>  out_5;   // channel 5 out
typedef output_t<PB10> out_6;   // channel 6 out
typedef output_t<PB11> out_7;   // channel 7 out

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

typedef button_t<PC14> btn_0;   // user button 0
typedef button_t<PC0>  btn_1;   // user button 1
typedef button_t<PC2>  btn_2;   // user button 2
typedef button_t<PA0>  btn_3;   // user button 3
typedef button_t<PA2>  btn_4;   // user button 4
typedef button_t<PC10> btn_5;   // user button 5
typedef button_t<PA12> btn_6;   // user button 6
typedef button_t<PF6>  btn_7;   // user button 7

typedef button_t<PA11> btn_8;   // user button 8
typedef button_t<PC9>  btn_9;   // user button 9
typedef button_t<PC7>  btn_10;  // user button 10
typedef button_t<PB6>  btn_11;  // encoder button

typedef encoder_t<3, PA6, PA7> encoder;

typedef st7789_t<1, PB3, PB5, PB4, PC12> display;

typedef timer::timer_t<6> aux;

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

void loop(text_renderer_t<display>& tr);

int main()
{
    out_0::setup();
    out_1::setup();
    out_2::setup();
    out_3::setup();
    out_4::setup();
    out_5::setup();
    out_6::setup();
    out_7::setup();

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

    encoder::setup<pull_up>(1 + (64 << 1));

    aux::setup(100, 1000);
    aux::update_interrupt_enable();

    const color_t fg = color::yellow;
    const color_t bg = color::red;

    display::setup();
    display::clear(bg);

    font_t ft = fontlib::cmunss_24;

    text_renderer_t<display> tr(ft, fg, bg);

    display::clear(bg);

    tr.set_pos(50, 50);
    tr.write("Hello Beats!");

    for (;;)
        loop(tr);
}

void loop(text_renderer_t<display>& tr)
{
    static unsigned last_count = -1;
    unsigned count = encoder::count() >> 1;

    if (count != last_count)
    {
        char buf[128];

        sprintf(buf, "%05u", count);
        tr.set_pos(50, 100);
        tr.write(buf);
        last_count = count;
    }

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

    if (led_0::read())
        out_0::toggle();

    if (led_1::read())
        out_1::toggle();

    if (led_2::read())
        out_2::toggle();

    if (led_3::read())
        out_3::toggle();

    if (led_4::read())
        out_4::toggle();

    if (led_5::read())
        out_5::toggle();

    if (led_6::read())
        out_6::toggle();

    if (led_7::read())
        out_7::toggle();

    sys_tick::delay_ms(1);
}

