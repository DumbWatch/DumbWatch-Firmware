/**
 * 
 */

#pragma once

#include <drivers/device.h>
#include <drivers/irq_handler.h>
#include <nrf52.h>

namespace NRF52
{

class Timer : public Device, public IRQHandler
{
public:
    enum class BitMode : uint8_t
    {
        Bits16 = 0,
        Bits8,
        Bits24,
        Bits32,
    };

    struct TimerConfiguration
    {
    };

public:
    Timer() = delete;
    Timer(uint32_t base_address, IRQn_Type irq_number)
    : Device(base_address), IRQHandler(irq_number) { m_peripheral = reinterpret_cast<NRF_TIMER_Type*>(base_address); }
    ~Timer() {}

    DeviceStatus initialize() override;
    DeviceStatus uninitialize() override;

    void handle_irq() override;

    // Timer specific functions
    void start();
    void stop();

    template<typename T>
    void set_max_ticks(T max_ticks);

    DeviceStatus set_prescaler(uint8_t prescaler);
    DeviceStatus set_bitmode(BitMode bitmode);

private:
    NRF_TIMER_Type* m_peripheral;
    uint8_t m_prescaler;
    bool m_running { false };
};

} // namespace NRF52