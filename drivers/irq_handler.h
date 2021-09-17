/**
 *
 */

#include <nrf52.h>
#include <cstdint>

namespace NRF52
{

class IRQHandler
{
public:
    virtual ~IRQHandler()
    {
        //uninstall_handler();
        NVIC_DisableIRQ(static_cast<IRQn_Type>(m_irq_number));
    }

    virtual void handle_interrupt() = 0;

protected:
    IRQHandler(uint8_t irq_number) : m_irq_number(irq_number)
    {
        //install_handler(irq_number, *this);
        NVIC_EnableIRQ(static_cast<IRQn_Type>(m_irq_number));
    }

private:
    uint8_t m_irq_number { 0 };
};

}
