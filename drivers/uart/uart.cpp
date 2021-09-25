/**
 * 
 */

#include <drivers/uart/uart.h>
#include <nrf52_bitfields.h>

namespace NRF52
{

static uint32_t s_baud_rates[16] = {
    0x0004F000,
    0x0009D000,
    0x0013B000,
    0x00275000,
    0x003AF000,
    0x004EA000,
    0x0075C000,
    0x009D0000,
    0x00EB0000,
    0x013A9000,
    0x01D60000,
    0x03B00000,
    0x04000000,
    0x07400000,
    0x0F000000,
    0x10000000
};

UART::UART(uint32_t base_address, const PinConfiguration& configuration)
: SerialDevice(base_address),
  IRQHandler(UARTE0_UART0_IRQn),
  m_configuration(configuration)
{
    m_uart = reinterpret_cast<NRF_UARTE_Type*>(base_address);
}

void UART::handle_irq()
{
    // Check what interrupt sources were generated
    if (m_uart->EVENTS_ENDTX == 1UL)
    {
        m_uart->EVENTS_ENDTX = 0UL;
        m_busy = false;
#if (USE_FREERTOS == 1)
        xSemaphoreGive(m_device_mutex);
#endif
    }
    else if (m_uart->EVENTS_ENDRX == 1UL)
    {
        m_uart->EVENTS_ENDRX = 0UL;
        m_busy = false;
    }
    else if (m_uart->EVENTS_ERROR == 1UL)
    {
        m_uart->EVENTS_ERROR = 0UL;
        m_busy = false;
    }
}

DeviceStatus UART::initialize()
{
    if (m_initialized)
        return DeviceStatus::ALREADY_INITIALIZED;

    if (m_busy)
        return DeviceStatus::DEVICE_BUSY;

    // Set pins from configuration
    m_uart->PSEL.TXD = m_configuration.txd_pin & 31;
    m_uart->PSEL.RXD = m_configuration.rxd_pin & 31;

    // Set pin configuration in GPIO block
    NRF_P0->PIN_CNF[m_uart->PSEL.RTS] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                        (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                                        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[m_uart->PSEL.TXD] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                        (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                                        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[m_uart->PSEL.CTS] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[m_uart->PSEL.RXD] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);

    NRF_P0->DIR |= (GPIO_DIR_PIN13_Output << GPIO_DIR_PIN13_Pos) | (GPIO_DIR_PIN15_Output << GPIO_DIR_PIN15_Pos);
    NRF_P0->OUTSET = (1UL << m_uart->PSEL.TXD);

    // Default baud rate to 9600bps
    m_uart->BAUDRATE = s_baud_rates[3];

    // Enable the UART
    m_uart->ENABLE = UARTE_ENABLE_ENABLE_Enabled;

    // Enable interrupt sources
    m_uart->INTENSET = (UARTE_INTEN_ENDRX_Enabled << UARTE_INTEN_ENDRX_Pos) |
                       (UARTE_INTEN_ENDTX_Enabled << UARTE_INTEN_ENDTX_Pos) |
                       (UARTE_INTEN_ERROR_Enabled << UARTE_INTEN_ERROR_Pos) |
                       (UARTE_INTEN_RXTO_Enabled << UARTE_INTEN_RXTO_Pos);

#if (USE_FREERTOS == 1)
    m_device_mutex = xSemaphoreCreateMutex();
#endif

    return DeviceStatus::SUCCESS;
}

DeviceStatus UART::uninitialize()
{
    // Stop any currently active transmissions
    abort();

    // Clear events
    m_uart->EVENTS_ERROR = 0UL;
    m_uart->EVENTS_ENDTX = 0UL;
    m_uart->EVENTS_ENDRX = 0UL;
    m_uart->EVENTS_RXDRDY = 0UL;
    m_uart->EVENTS_TXDRDY = 0UL;

    m_uart->ENABLE = UARTE_ENABLE_ENABLE_Disabled;

    m_uart->INTEN = 0UL;

    return DeviceStatus::UNIMPLEMENTED;
}

DeviceStatus UART::send(const uint8_t* data, size_t count)
{
    if (data == nullptr || count == 0UL)
        return DeviceStatus::INVALID_PARAMETER;

    if (m_busy)
        return DeviceStatus::DEVICE_BUSY;

#if (USE_FREERTOS == 1)
    if (xSemaphoreTake(m_device_mutex, 0) != pdTRUE)
        return DeviceStatus::DEVICE_BUSY;
#endif

    // Set up the EASYDMA pointers
    m_uart->TXD.PTR = reinterpret_cast<uint32_t>(data);
    m_uart->TXD.MAXCNT = count;

    // Do transfer
    m_uart->TASKS_STARTTX = 1UL;

    return DeviceStatus::SUCCESS;
}

DeviceStatus UART::receive(uint8_t* data, size_t count)
{
    if (data == nullptr || count == 0UL)
        return DeviceStatus::INVALID_PARAMETER;

    if (m_busy)
        return DeviceStatus::DEVICE_BUSY;

    m_uart->TXD.PTR = reinterpret_cast<uint32_t>(data);
    m_uart->TXD.MAXCNT = count;

    // Do transfer
    m_uart->TASKS_STARTTX = 1UL;
    return DeviceStatus::UNIMPLEMENTED;
}

DeviceStatus UART::transceive(const uint8_t*, uint8_t*, size_t)
{
    return DeviceStatus::UNIMPLEMENTED;
}

/////////////////////////////////////////////////////////////////////////////////////
DeviceStatus UART::set_baud_rate(BaudRate baud)
{
    if (m_busy)
        return DeviceStatus::DEVICE_BUSY;

    m_uart->BAUDRATE = s_baud_rates[static_cast<uint8_t>(baud)];

    return DeviceStatus::SUCCESS;
}

DeviceStatus UART::enable_hw_flow_control()
{
    if (!m_initialized)
        return DeviceStatus::NOT_INITIALIZED;

    if (m_busy)
        return DeviceStatus::DEVICE_BUSY;

    m_uart->PSEL.RTS = m_configuration.rts_pin & 31;
    m_uart->PSEL.CTS = m_configuration.cts_pin & 31;
    m_uart->CONFIG |= UARTE_CONFIG_HWFC_Enabled;

    return DeviceStatus::SUCCESS;
}

void UART::abort()
{
    // Abort any transfer in progress
    m_uart->TASKS_STOPRX = 1UL;
    m_uart->TASKS_STOPTX = 1UL;

    // Purge the RX buffer of any data
    m_uart->TASKS_FLUSHRX = 1UL;
}

} // namespace NRF52