/**
 *
 */

#include <nrf52.h>
#include <cstdint>

namespace NRF52
{

class IRQHandler
{
    static constexpr size_t NUMBER_OF_IRQS = 128;
    static IRQHandler* s_handlers[NUMBER_OF_IRQS];

public:
    virtual ~IRQHandler()
    {
        s_handlers[m_irq_number] = nullptr;
        NVIC_DisableIRQ(static_cast<IRQn_Type>(m_irq_number));
    }

    virtual void handle_irq() = 0;

protected:
    IRQHandler(uint8_t irq_number) : m_irq_number(irq_number)
    {
        s_handlers[irq_number] = this;
        NVIC_EnableIRQ(static_cast<IRQn_Type>(m_irq_number));
    }

private:
    static void  Default_Handler()
    {
        auto irqn = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
        auto* handler = s_handlers[irqn];
        
        if (handler != nullptr)
            handler->handle_irq();
    }

    uint8_t m_irq_number { 0 };
};

}
