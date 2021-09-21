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

        if (m_callback != nullptr)
            m_callback();
    }
}

DeviceStatus Timer::initialize()
{
    if (m_running)
        return DeviceStatus::DEVICE_BUSY;

    if (m_initialized)
        return DeviceStatus::ALREADY_INITIALIZED;

    // Stop the timer
    stop();

    // Clear task
    m_peripheral->TASKS_CLEAR = 1UL;

    // Enable interrupts for COMPARE[0]
    m_peripheral->INTENSET &= ~(0x3f << 16u);
    m_peripheral->INTENSET |= (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);

    m_initialized = true;

    return DeviceStatus::SUCCESS;
}

DeviceStatus Timer::uninitialize()
{
    return DeviceStatus::UNIMPLEMENTED;
}

DeviceStatus Timer::start()
{
    if (!m_initialized)
        return DeviceStatus::NOT_INITIALIZED;

    m_peripheral->TASKS_START = 1UL;
    m_running = true;

    return DeviceStatus::SUCCESS;
}

DeviceStatus Timer::stop()
{
    if (!m_initialized)
        return DeviceStatus::NOT_INITIALIZED;

    m_peripheral->TASKS_STOP = 1UL;
    m_running = false;

    return DeviceStatus::SUCCESS;
}

DeviceStatus Timer::set_bitmode(BitMode mode)
{
    if (!m_initialized)
        return DeviceStatus::NOT_INITIALIZED;

    if (m_running)
        return DeviceStatus::DEVICE_BUSY;

    m_peripheral->BITMODE &= ~TIMER_BITMODE_BITMODE_Msk;
    m_peripheral->BITMODE |= static_cast<uint8_t>(mode);

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