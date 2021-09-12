/**
 * 
 */

#include <cstdint>

int main(void)
{
    for(;;)
    {
        __asm__ volatile("wfi");
    }
}