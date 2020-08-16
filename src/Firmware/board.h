#pragma once

#include <gpio.h>
#include <button.h>
#include <pulse.h>
#include <timer.h>
#include <adc.h>
#include <dma.h>
#include <st7789.h>
#include <fifo.h>
#include <message.h>

namespace board
{
typedef tim_t<6> aux;

typedef pulse_t<PA3>  out0;     // channel 0 out
typedef pulse_t<PF4>  out1;     // channel 1 out
typedef pulse_t<PF5>  out2;     // channel 2 out
typedef pulse_t<PA4>  out3;     // channel 3 out
typedef pulse_t<PA5>  out4;     // channel 4 out
typedef pulse_t<PB2>  out5;     // channel 5 out
typedef pulse_t<PB10> out6;     // channel 6 out
typedef pulse_t<PB11> out7;     // channel 7 out

typedef pulse_t<PC13> led0;     // channel 0 led
typedef pulse_t<PC15> led1;     // channel 1 led
typedef pulse_t<PC1>  led2;     // channel 2 led
typedef pulse_t<PC3>  led3;     // channel 3 led
typedef pulse_t<PA1>  led4;     // channel 4 led
typedef pulse_t<PC11> led5;     // channel 5 led
typedef pulse_t<PA15> led6;     // channel 6 led
typedef pulse_t<PF7>  led7;     // channel 7 led

typedef pulse_t<PA8> ledA;      // user led 8
typedef pulse_t<PC8> ledB;      // user led 9
typedef pulse_t<PC6> ledC;      // user led 10

typedef pulse_t<PB8> ledX;      // rear red led
typedef pulse_t<PB9> ledY;      // rear yellow led

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

typedef encoder_t<3, PA6, PA7> enc;
typedef st7789_t<1, PB3, PB5, PB4, PC12> tft;
typedef fifo_t<message_t, 0, 8> mq;

typedef adc_t<1> adc;
typedef dma_t<1> adc_dma;
typedef tim_t<15> adc_tim;

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

    enc::setup<pull_up>(1 + (64 << 1));

    aux::setup(48-1, 1000-1); // 1kHz
    aux::enable_update_interrupt();
    interrupt::set<interrupt::TIM6_DAC>();

    adc_tim::setup(1, (sys_clock::freq() >> 1) / adc_sample_freq - 1);
    adc_tim::master_mode<adc_tim::mm_update>();
    adc_dma::setup();
    adc::setup();
    adc::sequence<8, 9, 14, 15>(); // PB0, PB1, PC4, PC5
    adc::dma<adc_dma, adc_dma_ch>(adc_buf, adc_buf_size);
    adc::trigger<0x4>(); // FIXME: use symbol for TIM15_TRIGO
    adc::enable();
    adc::start_conversion();

    tft::setup<fpclk_2>();
}

} // namespace board

template<typename BTN, typename MQ, int NO>
static void update()
{
    BTN::update();
    if (BTN::read())
    {
        MQ::put(message_t().emplace<button_press>(NO));
        board::ledX::pulse(100);
    }
}

template<> void handler<interrupt::TIM6_DAC>()
{
    using namespace board;

    aux::clear_update_interrupt_flag();

    out0::update();
    out1::update();
    out2::update();
    out3::update();
    out4::update();
    out5::update();
    out6::update();
    out7::update();

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

    if (btnE::update_read())
        mq::put(message_t().emplace<encoder_press>(unit));

    static int16_t enc_last_count = 0;
    int16_t c = static_cast<int16_t>(enc::count()) >> 1;

    if (c != enc_last_count)
    {
        int16_t n = c - enc_last_count;

        // hack around weird glitch

        if (n == 64)
            n = -1;
        else if (n == -64)
            n = 1;

        mq::put(message_t().emplace<encoder_delta>(n));
        enc_last_count = c;
        ledY::pulse(25);
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
}

