/**
 * 
 */

#include <FreeRTOS.h>
#include <thread.h>
#include <drivers/uart/uart.h>

static NRF52::UART uart0(NRF_UARTE0_BASE, { 5, 6, 7, 8 });

class MyThread : public FreeRTOS::Thread
{
public:
	MyThread(const char* name) : Thread(name, 128)
	{
		start();
	}

protected:
	void task()
	{
		uint8_t msg[17] = "Hello, World!\r\n\0";
		for(;;)
		{
				if (uart0.send(&msg[0], sizeof(msg)) == NRF52::DeviceStatus::DEVICE_BUSY)
					continue;

				delay(1000);
		}
	}
};

int main(void)
{
	MyThread thread("Test");

	// Initialize UART
	uart0.initialize();
	uart0.set_baud_rate(NRF52::UART::BaudRate::Baud115200);
	uart0.enable_hw_flow_control();

    vTaskStartScheduler();

	for(;;){}
	return 0;
}