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
#include <cstdlib>

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


// TODO: Optimise me with assembly please!
extern "C" void* memcpy(void* src, void* dst, size_t num)
{
    // r0 = src
    // r1 = dst
    // r2 = num

    // If this is the case, we can do a word copy
    if((reinterpret_cast<uint32_t>(src) & 3) == 0 && (reinterpret_cast<uint32_t>(dst) & 3) == 0 && (num & 3) == 0)
    {
        __asm__ volatile(
        "copy:\n"
        "ldr r3, [r0], #4\n" // Load r3 with src[offset]. src += 4
        "str r3, [r1], #4\n" // Store r3 into src[offset]. dst += 4
        "subs r2, r2, #4\n"  // num -= 4
        "bgt copy\n");
    }
    else
    {
        for(size_t i = 0; i < num; i++)
            *reinterpret_cast<uint8_t*>(dst) = *reinterpret_cast<uint8_t*>(src);
    }

    return dst;
}

extern "C" void* memset(void* ptr, [[maybe_unused]]int value, [[maybe_unused]]size_t num)
{
    // r0 = ptr
    // r1 = value
    // r2 = num
    __asm__ volatile(
    "move:"
    "strb r1, [r0], #1\n"
    "subs r2, r2, #1\n"
    "bgt move\n");

    return ptr;
}

extern "C" int memcmp(void* src, void* target, size_t num)
{
    const uint8_t* str_ptr = reinterpret_cast<uint8_t*>(src);
    const uint8_t* target_ptr = reinterpret_cast<uint8_t*>(target);

    for(int i = 0; i < static_cast<int>(num); ++i)
    {
        if(*(str_ptr + i) < *(target_ptr + i))
            return -1;
        if(*(str_ptr + i) > *(target_ptr + i))
            return 1;
    }

    return 0;
}

extern "C" void* malloc(size_t)
{
    return nullptr;
}