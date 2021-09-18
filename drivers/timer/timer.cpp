/**
 * 
 */

#include <drivers/timer/timer.h>
#include <nrf52_bitfields.h>

namespace NRF52
{

void Timer::handle_irq()
{
    if ((m_peripheral->EVENTS_COMPARE[0] != 0) && ((m_peripheral->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
    {
        m_peripheral->EVENTS_COMPARE[0] = 0UL;
    }
}

DeviceStatus Timer::initialize()
{
    // Stop the timer
    stop();

    // Clear task
    m_peripheral->TASKS_CLEAR = 1UL;

    // Enable interrupts for COMPARE[0]
    m_peripheral->INTENSET &= ~(0x3f << 16u);
    m_peripheral->INTENSET |= (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);

    return DeviceStatus::SUCCESS;
}

DeviceStatus Timer::uninitialize()
{
    return DeviceStatus::SUCCESS;
}

void Timer::start()
{
    m_peripheral->TASKS_START = 1UL;
    m_running = true;
}

void Timer::stop()
{
    m_peripheral->TASKS_STOP = 1UL;
    m_running = false;
}

DeviceStatus Timer::set_bitmode([[maybe_unused]] BitMode mode)
{
    return DeviceStatus::SUCCESS;
}

DeviceStatus Timer::set_prescaler(uint8_t prescaler)
{
    m_peripheral->PRESCALER = (prescaler & 0xf);
    return DeviceStatus::SUCCESS;
}

template<>
void Timer::set_max_ticks(uint16_t max_ticks)
{
    m_peripheral->CC[0] = max_ticks;
}

} // namespace NRF52