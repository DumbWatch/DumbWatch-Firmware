/**
 * 
 * 
 */

#pragma once

#ifndef USE_FREERTOS
    #error "USE_FREERTOS isn't defined (is this a FreeRTOS project?)"
#endif

#include <FreeRTOS.h>
#include <task.h>

namespace FreeRTOS
{

class Thread
{
public:
    enum class ThreadResult
    {
        SUCCESS = 0,
        ERROR = -1,
        ALREADY_STARTED = -2,
    };

public:
    Thread() = delete; // Default constructor is deleted
    Thread(const char* task_name, configSTACK_DEPTH_TYPE stack_depth);
    Thread(const char* task_name, configSTACK_DEPTH_TYPE stack_depth, UBaseType_t priority);
    ~Thread();

    Thread(const Thread& thread) = delete;  // A thread cannot be copied
    Thread(Thread&& thread) = delete;       // A thread cannot be moved

public:
    ThreadResult start();

    void suspend();
    void resume();
    void delay(const TickType_t ticks_to_delay) const;
    void delay_until(const TickType_t time_increment);

    bool running() const { return m_running; }
    const char* task_name() const { return &m_thread_name[0]; }

protected:
    virtual void task() = 0;
    
private:
    static void task_runner(void* arguments);

private:
    // Thread info
    TaskHandle_t m_thread { nullptr };
    char m_thread_name[configMAX_TASK_NAME_LEN] = {'\0'};
    configSTACK_DEPTH_TYPE m_stack_depth { 0 };
    UBaseType_t m_priority { 0 };

    // Thread state info
    bool m_running { false };
    TickType_t m_last_wake_ticks { 0 };
};




}