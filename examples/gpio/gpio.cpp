/**
 *  Flash LED1
 */
#include <nrf52.h>
#include <nrf52_bitfields.h>
#include <cstdint>
#include <drivers/gpio/pin.h>

static constexpr uint32_t COUNTER_RESET_VALUE = 0x7FFFFF;

int main(void)
{
    // LED1 is connected to P0.17!

    NRF52::Pin led_pin(17);
    led_pin.set_direction(NRF52::Pin::Direction::Output);
    led_pin.set_pull_mode(NRF52::Pin::PullMode::Disabled);
    led_pin.set_drive_strength(NRF52::Pin::DriveStrength::S0S1);
    led_pin.set_input_buffer_mode(NRF52::Pin::InputConnection::Disconnect);

    uint32_t counter = COUNTER_RESET_VALUE;
    for(;;)
    {
        // Set Pin17 HIGH
        led_pin.set_high();
        while(counter--);
        counter = COUNTER_RESET_VALUE;

        // Set Pin17 LOW
        led_pin.set_low();
        while(counter--);
        counter = COUNTER_RESET_VALUE;
    }
}