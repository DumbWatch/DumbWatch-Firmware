/**
 * 
 */
#include <drivers/gpio/pin.h>

#include <nrf52.h>
#include <nrf52_bitfields.h>

namespace NRF52 {

void Pin::set_direction(Pin::Direction direction)
{
    NRF_P0->PIN_CNF[m_pin] &= ~GPIO_PIN_CNF_DIR_Msk;
    NRF_P0->PIN_CNF[m_pin] |= static_cast<uint32_t>(direction) << GPIO_PIN_CNF_DIR_Pos;
}

void Pin::set_input_buffer_mode(Pin::InputConnection mode)
{
    NRF_P0->PIN_CNF[m_pin] &= ~GPIO_PIN_CNF_INPUT_Msk;
    NRF_P0->PIN_CNF[m_pin] |= static_cast<uint32_t>(mode) << GPIO_PIN_CNF_INPUT_Pos;
}

void Pin::set_drive_strength(Pin::DriveStrength strength)
{
    NRF_P0->PIN_CNF[m_pin] &= ~GPIO_PIN_CNF_DRIVE_Msk;
    NRF_P0->PIN_CNF[m_pin] |= static_cast<uint32_t>(strength) << GPIO_PIN_CNF_DRIVE_Pos;
}

void Pin::set_pull_mode(Pin::PullMode mode)
{
    NRF_P0->PIN_CNF[m_pin] &= ~GPIO_PIN_CNF_PULL_Msk;
    NRF_P0->PIN_CNF[m_pin] |= static_cast<uint32_t>(mode) << GPIO_PIN_CNF_PULL_Pos;
}

void Pin::set_sense_mode(Pin::Sense mode)
{
    NRF_P0->PIN_CNF[m_pin] &= ~GPIO_PIN_CNF_SENSE_Msk;
    NRF_P0->PIN_CNF[m_pin] |= static_cast<uint32_t>(mode) << GPIO_PIN_CNF_SENSE_Pos;
}

void Pin::set_high()
{
    NRF_P0->OUTSET |= (1UL << m_pin);
}

void Pin::set_low()
{
    NRF_P0->OUTCLR |= (1UL << m_pin);
}

uint8_t Pin::read() const
{
    uint8_t ret = (NRF_P0->IN >> m_pin) & 1UL;

    return ret;
}

}