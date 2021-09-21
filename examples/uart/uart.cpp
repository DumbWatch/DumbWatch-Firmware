/**
 *  Flash LED1
 */
#include <cstdint>
#include <cstring>
#include <drivers/timer/timer.h>
#include <drivers/uart/uart.h>
#include <nrf52.h>
#include <nrf52_bitfields.h>

static NRF52::UART uart(NRF_UARTE0_BASE, { 5, 6, 7, 8 });
static NRF52::Timer timer0(NRF_TIMER0_BASE, TIMER0_IRQn);

void send_uart_message()
{
    uint8_t msg[17] = "Hello, World!\r\n\0";
    uart.send(reinterpret_cast<const uint8_t*>(&msg[0]), 15);
}

void init()
{
    uart.initialize();
    uart.set_baud_rate(NRF52::UART::BaudRate::Baud115200); // 11.52bps
    uart.enable_hw_flow_control();

    timer0.initialize();
    timer0.set_bitmode(NRF52::Timer::BitMode::Bits16);
    timer0.set_max_ticks<uint16_t>(0xffff);
    timer0.set_prescaler(6);
    timer0.set_callback(&send_uart_message);
    timer0.start();
}

int main(void)
{
    init();

    for (;;)
    {
        __WFI();
    }
}
