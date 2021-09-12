/**
 *  Flash LED1
 */
#include <nrf52.h>
#include <nrf52_bitfields.h>
#include <cstdint>

static constexpr uint16_t COUNTER_RESET_VALUE = 0xABCD;

int main(void)
{
    // LED1 is connected to P0.17!

    // Configure Pin17 as output, Pull up/down disabled and standard drive strength
    NRF_P0->PIN_CNF[17] =   (GPIO_PIN_CNF_DIR_Output        << GPIO_PIN_CNF_DIR_Pos) | \
                            (GPIO_PIN_CNF_PULL_Disabled     << GPIO_PIN_CNF_PULL_Pos);

    // Set Pin17 as output
    NRF_P0->DIR |=          (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
    
    uint16_t counter = COUNTER_RESET_VALUE;
    for(;;)
    {
        // Set Pin17 HIGH
        NRF_P0->OUTSET |= (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
        while(counter--);
        counter = COUNTER_RESET_VALUE;

        // Set Pin17 LOW
        NRF_P0->OUTCLR |= (GPIO_DIR_PIN17_Output << GPIO_DIR_PIN17_Pos);
        while(counter--);
        counter = COUNTER_RESET_VALUE;
    }
}