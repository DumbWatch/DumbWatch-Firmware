/**
 * 
 */

#include <FreeRTOS.h>
#include <task.h>
#include <drivers/uart/uart.h>

static NRF52::UART uart0(NRF_UARTE0_BASE, { 5, 6, 7, 8 });

void thread_fn([[maybe_unused]]void* params)
{
	uint8_t msg[17] = "Hello, World!\r\n\0";
	for(;;)
	{
            if (uart0.send(&msg[0], sizeof(msg)) == NRF52::DeviceStatus::DEVICE_BUSY)
                continue;

            vTaskDelay(1000);
	}
}

void thread2_fn([[maybe_unused]] void* params)
{
    uint8_t msg[18] = "Hello, World2!\r\n\0";
    for (;;)
    {
        if (uart0.send(&msg[0], sizeof(msg)) == NRF52::DeviceStatus::DEVICE_BUSY)
            continue;

        vTaskDelay(2000);
    }
}

int main(void)
{
	// Initialize UART
	uart0.initialize();
	uart0.set_baud_rate(NRF52::UART::BaudRate::Baud115200);
	uart0.enable_hw_flow_control();

	xTaskCreate(&thread_fn, "Thread1", 128, nullptr, 5, NULL);
        xTaskCreate(&thread2_fn, "Thread2", 128, nullptr, 5, NULL);

        vTaskStartScheduler();
	for(;;){}
	return 0;
}