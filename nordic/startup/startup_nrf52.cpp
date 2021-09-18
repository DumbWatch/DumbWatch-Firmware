/*

Copyright (c) 2009-2021 ARM Limited. All rights reserved.

    SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the License); you may
not use this file except in compliance with the License.
You may obtain a copy of the License at

    www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Modifications (c) 2021 Jesse Buhagiar

*/

#include <nrf52.h>

#define __SYSTEM_CLOCK_64M      (64000000UL)

#if defined ( __CC_ARM )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK_64M;
#elif defined ( __ICCARM__ )
    __root uint32_t SystemCoreClock = __SYSTEM_CLOCK_64M;
#elif defined ( __GNUC__ )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK_64M;
#endif

extern "C" int main(void);

extern "C" void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK_64M;
}

extern "C" void SystemInit(void)
{
	SystemCoreClockUpdate();
}

extern "C" int main(void);

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

typedef void (*func_ptr_t)();

extern func_ptr_t __preinit_array_start[];
extern func_ptr_t __preinit_array_end[];
extern func_ptr_t __init_array_start[];
extern func_ptr_t __init_array_end[];
extern func_ptr_t __fini_array_start[];
extern func_ptr_t __fini_array_end[];

static void _copy_data_section()
{
    uint32_t* dest_word  = &__data_start__;
    uint32_t* load_word = &__etext;
    while(dest_word < &__data_end__)
    {
        *dest_word++ = *load_word++;
    }
}

static void _zero_bss()
{
    uint32_t* curr_address = &__bss_start__;
    while(curr_address < &__bss_end__)
    {
        *curr_address++ = 0u;
    }
}

static void _cxx_global_ctors()
{
    unsigned count = __preinit_array_end - __preinit_array_start;
    for (auto idx = 0u; idx < count; idx++)
    {
        func_ptr_t ctor = __preinit_array_start[idx];
        (*ctor)();
    }

    count = __init_array_end - __init_array_start;
    for (auto idx = 0u; idx < count; idx++)
    {
        func_ptr_t ctor = __init_array_start[idx];
        (*ctor)();
    }
}

static void _cxx_global_dtors()
{
    unsigned count = __init_array_end - __init_array_start;
    for (auto idx = 0u; idx < count; idx++)
    {
        func_ptr_t dtor = __init_array_start[idx];
        (*dtor)();
    }
}


extern "C" void _start(void)
{
    _copy_data_section();   // Copy data section from image to SRAM
    _zero_bss();            // Zero out BSS section
    _cxx_global_ctors();    // Call all global constructors

    static_cast<void>(main()); // Call main with no arguments and discard return value

    _cxx_global_dtors();    // Call all global destructors

    for(;;)
    {
        __asm__ volatile("wfi"); // Spin forever if we return from main (however that happens...)
    }
}

extern "C" void _fini(void)
{
    for(;;)
    {
        __asm__ volatile("wfi");
    }
}
