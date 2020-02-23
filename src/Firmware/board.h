#pragma once

#include <gpio.h>
#include <button.h>
#include <timer.h>
#include <adc.h>
#include <dma.h>
#include <st7789.h>
#include <fifo.h>
#include <message.h>

namespace board
{
using namespace hal::gpio;

template<gpio_pin_t PIN>
struct led_t: output_t<PIN>
{
    static void set_ms(uint16_t n)
    {
        output_t<PIN>::set();
        m_count = n;
    }

    static void update()
    {
        if (m_count > 0)
        {
            if (--m_count == 0)
                output_t<PIN>::clear();
        }
    }

    static uint16_t m_count;
};

template<gpio_pin_t PIN>
uint16_t led_t<PIN>::m_count = 0;

typedef hal::timer::timer_t<6> aux;

typedef output_t<PA3>  out0;    // channel 0 out
typedef output_t<PF4>  out1;    // channel 1 out
typedef output_t<PF5>  out2;    // channel 2 out
typedef output_t<PA4>  out3;    // channel 3 out
typedef output_t<PA5>  out4;    // channel 4 out
typedef output_t<PB2>  out5;    // channel 5 out
typedef output_t<PB10> out6;    // channel 6 out
typedef output_t<PB11> out7;    // channel 7 out

typedef led_t<PC13> led0;       // channel 0 led
typedef led_t<PC15> led1;       // channel 1 led
typedef led_t<PC1>  led2;       // channel 2 led
typedef led_t<PC3>  led3;       // channel 3 led
typedef led_t<PA1>  led4;       // channel 4 led
typedef led_t<PC11> led5;       // channel 5 led
typedef led_t<PA15> led6;       // channel 6 led
typedef led_t<PF7>  led7;       // channel 7 led

typedef led_t<PA8> ledA;        // user led 8
typedef led_t<PC8> ledB;        // user led 9
typedef led_t<PC6> ledC;        // user led 10

typedef led_t<PB8> ledX;        // rear red led
typedef led_t<PB9> ledY;        // rear yellow led

typedef button_t<PC14> btn0;    // user button 0
typedef button_t<PC0>  btn1;    // user button 1
typedef button_t<PC2>  btn2;    // user button 2
typedef button_t<PA0>  btn3;    // user button 3
typedef button_t<PA2>  btn4;    // user button 4
typedef button_t<PC10> btn5;    // user button 5
typedef button_t<PA12> btn6;    // user button 6
typedef button_t<PF6>  btn7;    // user button 7

typedef button_t<PA11> btnA;    // user button 8
typedef button_t<PC9>  btnB;    // user button 9
typedef button_t<PC7>  btnC;    // user button 10
typedef button_t<PB6>  btnE;    // encoder button

typedef input_t<PB7> clk;       // clock input
typedef input_t<PD2> rst;       // reset input

typedef hal::timer::encoder_t<3, PA6, PA7> enc;
typedef st7789::st7789_t<1, PB3, PB5, PB4, PC12> tft;
typedef fifo_t<message_t, 0, 8> mq;

typedef hal::adc::adc_t<1> adc;
typedef hal::dma::dma_t<1> adc_dma;
typedef hal::timer::timer_t<15> adc_tim;

static const uint8_t adc_dma_ch = 1;
static const uint8_t adc_buf_size = 4;
static uint16_t adc_buf[adc_buf_size];
static const uint32_t adc_sample_freq = 2000;

template<uint8_t CH> inline uint16_t cvin();

template<> inline uint16_t cvin<0>() { return 4095 - adc_buf[2]; }
template<> inline uint16_t cvin<1>() { return 4095 - adc_buf[1]; }
template<> inline uint16_t cvin<2>() { return 4095 - adc_buf[3]; }
template<> inline uint16_t cvin<3>() { return 4095 - adc_buf[0]; }

void setup()
{
    out0::setup();
    out1::setup();
    out2::setup();
    out3::setup();
    out4::setup();
    out5::setup();
    out6::setup();
    out7::setup();

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

    ledX::setup();
    ledY::setup();

    btn0::setup<pull_up>();
    btn1::setup<pull_up>();
    btn2::setup<pull_up>();
    btn3::setup<pull_up>();
    btn4::setup<pull_up>();
    btn5::setup<pull_up>();
    btn6::setup<pull_up>();
    btn7::setup<pull_up>();

    btnA::setup<pull_up>();
    btnB::setup<pull_up>();
    btnC::setup<pull_up>();
    btnE::setup<pull_up>();

    clk::setup();
    rst::setup();

    clk::enable_interrupt<falling_edge>();
    rst::enable_interrupt<falling_edge>();
    hal::nvic<interrupt::EXTI4_15>::enable();

    enc::setup<pull_up>(1 + (64 << 1));

    aux::setup(48-1, 1000-1); // 1kHz
    aux::update_interrupt_enable();
    hal::nvic<interrupt::TIM6_DAC>::enable();

    adc_tim::setup(1, (hal::sys_clock::freq() >> 1) / adc_sample_freq - 1);
    adc_tim::master_mode<adc_tim::mm_update>();
    adc_dma::setup();
    adc::setup();
    adc::sequence<8, 9, 14, 15>(); // PB0, PB1, PC4, PC5
    adc::dma<adc_dma, adc_dma_ch>(adc_buf, adc_buf_size);
    adc::trigger<0x4>(); // FIXME: use symbol for TIM15_TRIGO
    adc::enable();
    adc::start_conversion();

    tft::setup<hal::spi::fpclk_2>();
}

} // namespace board

template<typename BTN, typename MQ, int NO>
static void update()
{
    BTN::update();
    if (BTN::read())
    {
        MQ::put(message_t().emplace<button_press>(NO));
        board::ledX::set_ms(100);
    }
}

template<> void handler<interrupt::TIM6_DAC>()
{
    using namespace board;

    out0::set();
    aux::clear_uif();

    update<btn0, mq, 0>();
    update<btn1, mq, 1>();
    update<btn2, mq, 2>();
    update<btn3, mq, 3>();
    update<btn4, mq, 4>();
    update<btn5, mq, 5>();
    update<btn6, mq, 6>();
    update<btn7, mq, 7>();

    update<btnA, mq, 10>();
    update<btnB, mq, 11>();
    update<btnC, mq, 12>();
    update<btnE, mq, 13>();

    static int16_t enc_last_count = 0;
    int16_t c = static_cast<int16_t>(enc::count()) >> 1;

    if (c != enc_last_count)
    {
        mq::put(message_t().emplace<encoder_delta>(enc_last_count - c));
        enc_last_count = c;
        ledY::set_ms(25);
    }

    led0::update();
    led1::update();
    led2::update();
    led3::update();
    led4::update();
    led5::update();
    led6::update();
    led7::update();
    ledA::update();
    ledB::update();
    ledC::update();
    ledX::update();
    ledY::update();

    out0::clear();
}

template<> void handler<interrupt::EXTI4_15>()
{
    using namespace board;
    extern void clock_trigger();
    extern void reset_trigger();

    bool ci = clk::interrupt_pending();
    bool ri = rst::interrupt_pending();

    if (ci)
        clock_trigger();
    if (ri)
        reset_trigger();
    if (ci)
        clk::clear_interrupt();
    if (ri)
        rst::clear_interrupt();
}

