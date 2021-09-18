/**
 * 
 */

#include <drivers/irq_handler.h>

namespace NRF52
{

static IRQHandler* s_handlers[IRQHandler::NUMBER_OF_IRQS];

IRQHandler::IRQHandler(uint8_t irq_number)
    : m_irq_number(irq_number)
{
    s_handlers[irq_number] = this;
    NVIC_EnableIRQ(static_cast<IRQn_Type>(m_irq_number));
}

IRQHandler::~IRQHandler()
{
    s_handlers[m_irq_number] = nullptr;
    NVIC_DisableIRQ(static_cast<IRQn_Type>(m_irq_number));
}

void IRQHandler::Default_Handler()
{
    auto irqn = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
    auto* handler = s_handlers[irqn];
    
    if (handler != nullptr)
        handler->handle_irq();
}

}