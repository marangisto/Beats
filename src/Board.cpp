#include "Board.h"
#include <fontlib.h>
#include <text.h>

using hal::sys_tick;
using namespace hal::timer;
using namespace hal::gpio;
using namespace st7789;
using namespace fontlib;

typedef hal::timer::timer_t<6> aux;

extern "C" void ISR_TIM6_DAC(void)
{
    aux::clear_uif();

    btn0::update();
    btn1::update();
    btn2::update();
    btn3::update();
    btn4::update();
    btn5::update();
    btn6::update();
    btn7::update();

    btnA::update();
    btnB::update();
    btnC::update();
    btnE::update();
}

void initialize_board()
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

    encoder::setup<pull_up>(1 + (64 << 1));

    aux::setup(100, 1000);
    aux::update_interrupt_enable();

    display::setup();
    display::clear(color::red);

    text_renderer_t<display> tr(fontlib::cmunss_24, color::white, color::red, true);

    tr.write("Beats 0.0");
    sys_tick::delay_ms(1000);
}

