/**
 * 
 * 
 */

#include <thread.h>
#include <cstring>

namespace FreeRTOS
{

Thread::Thread(const char* task_name, configSTACK_DEPTH_TYPE stack_depth)
    : m_thread(nullptr),
      m_stack_depth(stack_depth),
      m_priority(_PRIO_APP_MID),
      m_running(false),
      m_last_wake_ticks(0)
{
    strncpy(&m_thread_name[0], &task_name[0], configMAX_TASK_NAME_LEN);
}

Thread::Thread(const char* task_name, configSTACK_DEPTH_TYPE stack_depth, UBaseType_t priority)
    : m_thread(nullptr),
      m_stack_depth(stack_depth),
      m_priority(priority),
      m_running(false),
      m_last_wake_ticks(0)
{
    strncpy(&m_thread_name[0], &task_name[0], configMAX_TASK_NAME_LEN);
}

Thread::~Thread()
{

}

Thread::ThreadResult Thread::start()
{
    if (m_running)
        return ThreadResult::ALREADY_STARTED;

    auto task_result = xTaskCreate(&Thread::task_runner, 
                                    m_thread_name, 
                                    m_stack_depth, 
                                    this,
                                    m_priority,
                                    nullptr);
    if (task_result != pdPASS)
        return ThreadResult::ERROR;

    return ThreadResult::SUCCESS;
}

void Thread::suspend()
{
    m_running = true;
    vTaskSuspend(m_thread);
}

void Thread::resume()
{
    m_running = true;
    vTaskResume(m_thread);
}

void Thread::delay(const TickType_t ticks_to_delay) const
{
    vTaskDelay(ticks_to_delay);
}

void Thread::delay_until(const TickType_t time_increment)
{
    vTaskDelayUntil(&m_last_wake_ticks, time_increment);
}

void Thread::task_runner(void* parameters)
{
    auto* thread = static_cast<Thread*>(parameters);
    thread->task();

    // If the task returns then let's delete the thread
    vTaskDelete(thread->m_thread);
    thread->m_thread = nullptr;
}


}

