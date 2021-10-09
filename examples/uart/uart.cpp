/**
 *  Flash LED1
 */
#include <cstdint>
#include <cstring>
#include <drivers/uart/uart.h>
#include <nrf52.h>
#include <nrf52_bitfields.h>

using namespace NRF52;

static UART uart(NRF_UARTE0_BASE, { 5, 6, 7, 8 });

static bool receive_complete = false;

void uart_callback(uint32_t events)
{
    if ((events & UART::EVENT_RECEIVE_COMPLETE) == UART::EVENT_RECEIVE_COMPLETE)
    {
        receive_complete = true;
    }
}

void init()
{
    uart.initialize();
    uart.set_baud_rate(NRF52::UART::BaudRate::Baud115200); // 11.52bps
    uart.set_callback(&uart_callback);
}

int main(void)
{
    uint8_t msg[17] = "Hello, World!\r\n\0";
    uint8_t recv_buf[15];
    uint32_t counter = 0xfffff;

    init();

    for (;;)
    {
        receive_complete = false;
        uart.send(reinterpret_cast<const uint8_t*>(&msg[0]), 15);
        uart.receive(&recv_buf[0], 15);

        while (receive_complete == false);
        while(counter--);

        counter = 0xfffff;
    }
}
