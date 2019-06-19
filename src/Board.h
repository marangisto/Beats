#pragma once

#include <gpio.h>
#include <button.h>
#include <timer.h>
#include <st7789.h>

using namespace hal::gpio;

typedef output_t<PA3>  out0;    // channel 0 out
typedef output_t<PF4>  out1;    // channel 1 out
typedef output_t<PF5>  out2;    // channel 2 out
typedef output_t<PA4>  out3;    // channel 3 out
typedef output_t<PA5>  out4;    // channel 4 out
typedef output_t<PB2>  out5;    // channel 5 out
typedef output_t<PB10> out6;    // channel 6 out
typedef output_t<PB11> out7;    // channel 7 out

typedef output_t<PC13> led0;    // channel 0 led
typedef output_t<PC15> led1;    // channel 1 led
typedef output_t<PC1>  led2;    // channel 2 led
typedef output_t<PC3>  led3;    // channel 3 led
typedef output_t<PA1>  led4;    // channel 4 led
typedef output_t<PC11> led5;    // channel 5 led
typedef output_t<PA15> led6;    // channel 6 led
typedef output_t<PF7>  led7;    // channel 7 led

typedef output_t<PA8> ledA;     // user led 8 
typedef output_t<PC8> ledB;     // user led 9 
typedef output_t<PC6> ledC;     // user led 10 

typedef output_t<PB8> ledX;     // rear red led
typedef output_t<PB9> ledY;     // rear yellow led

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

typedef hal::timer::encoder_t<3, PA6, PA7> encoder;

typedef st7789::st7789_t<1, PB3, PB5, PB4, PC12> display;

void initialize_board();

