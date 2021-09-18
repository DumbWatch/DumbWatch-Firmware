/**
 *
 */

#include <nrf52.h>
#include <cstdint>
#include <cstdlib>

namespace NRF52
{

class IRQHandler
{
public:
    static constexpr size_t NUMBER_OF_IRQS = 128;

public:
    virtual ~IRQHandler();

    virtual void handle_irq() = 0;

protected:
    IRQHandler(uint8_t irq_number);

private:
    uint8_t m_irq_number { 0 };
};

}
