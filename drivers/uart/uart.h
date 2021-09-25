/**
 * 
 */

#include <drivers/irq_handler.h>
#include <drivers/serial_device.h>
#include <nrf52.h>

namespace NRF52
{

//
// NOTE: The NRF52832 contains only a single UART module
//
class UART : public SerialDevice, public IRQHandler
{
public:
    // FIXME: Should this be part of the driver model???
    struct PinConfiguration
    {
        uint8_t rts_pin; // Pin used for RTS signal
        uint8_t txd_pin; // Pin used for data TX
        uint8_t cts_pin; // Pin used for CTS signal
        uint8_t rxd_pin; // Pin used for data RX
    };

    enum class BaudRate : uint8_t
    {
        Baud1200 = 0,
        Baud2400,
        Baud4800,
        Baud9600,
        Baud14400,
        Baud19200,
        Baud28800,
        Baud38400,
        Baud57600,
        Baud76800,
        Baud115200,
        Baud230400,
        Baud250000,
        Baud460800,
        Baud921600,
        Baud1M
    };

public:
    UART() = delete;
    UART(uint32_t base_address, const PinConfiguration& configuration);
    ~UART() { uninitialize(); }

    DeviceStatus initialize() override;
    DeviceStatus uninitialize() override;
    DeviceStatus receive(uint8_t* data, size_t count) override;
    DeviceStatus send(const uint8_t* data, size_t count) override;
    DeviceStatus transceive(const uint8_t* txbuf, uint8_t* rxbuf, size_t count) override;

    // UART specific functions (not part of the driver model)
    DeviceStatus set_baud_rate(BaudRate baud);
    DeviceStatus enable_hw_flow_control();
    void abort();

public:
    void handle_irq() override;

private:
    NRF_UARTE_Type* m_uart; // Pointer to our peripheral
    BaudRate m_baud_rate;   // Current baud rate
    PinConfiguration m_configuration;
    bool m_busy { false }; // Is the device currently busy?
};

} // namespace NRF52