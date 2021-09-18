/**
 *  Flash LED1
 */
#include <cstdint>
#include <drivers/timer/timer.h>
#include <nrf52.h>
#include <nrf52_bitfields.h>

static NRF52::Timer timer0(NRF_TIMER0_BASE, TIMER0_IRQn);

int main(void)
{

    timer0.initialize();
    timer0.set_bitmode(NRF52::Timer::BitMode::Bits16);
    timer0.set_max_ticks<uint16_t>(0xffff);
    timer0.set_prescaler(0xf);
    timer0.start();

    for (;;)
    {
        __WFE();
    }
}