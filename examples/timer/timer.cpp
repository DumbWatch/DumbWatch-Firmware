/**
 *  Flash LED1
 */
#include <cstdint>
#include <drivers/timer/timer.h>
#include <nrf52.h>
#include <nrf52_bitfields.h>

static NRF52::Timer timer0(NRF_TIMER0_BASE, TIMER0_IRQn);
static bool led_on = false;

void toggle_led()
{
    if (led_on)
    {
        NRF_P0->OUTCLR |= (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
    }
    else
    {
        NRF_P0->OUTSET |= (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
    }

    led_on = !led_on;
}

void setup_led()
{
    // Configure Pin17 as output, Pull up/down disabled and standard drive strength
    NRF_P0->PIN_CNF[17] =   (GPIO_PIN_CNF_DIR_Output        << GPIO_PIN_CNF_DIR_Pos) | \
                            (GPIO_PIN_CNF_PULL_Disabled     << GPIO_PIN_CNF_PULL_Pos);

    // Set Pin17 as output
    NRF_P0->DIR |=          (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
}

int main(void)
{
    setup_led();

    timer0.initialize();
    timer0.set_bitmode(NRF52::Timer::BitMode::Bits16);
    timer0.set_max_ticks<uint16_t>(0xffff);
    timer0.set_prescaler(6);
    timer0.set_callback(&toggle_led);
    timer0.start();

    for (;;)
    {
        __WFE();
    }
}
