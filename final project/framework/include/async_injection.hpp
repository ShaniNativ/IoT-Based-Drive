/*** 
Written by: Shani Davidian
Reviewed by: Idan
Date: 25.3.25
Exercise: Async-Injection
***/

#ifndef ILRD_RD1645_ASYNC_INJECTION_HPP
#define ILRD_RD1645_ASYNC_INJECTION_HPP

#include <functional> /* std::function */

#include <handleton.hpp>
#include <scheduler.hpp>

namespace ilrd
{

class AsyncInjection
{
class AsyncInjectionTask;
public:
    //function return true if task is reaccuring task, otherwise return false;
    AsyncInjection(std::function<bool()> function, std::chrono::milliseconds interval);

private: 
    ~AsyncInjection() = default; 
    void PerformAction(); 
    std::function<bool()> m_function; 
    std::chrono::milliseconds m_interval; 
    std::shared_ptr<ilrd::scheduler::ISchedulerTask> m_async_injection_task; 

    class AsyncInjectionTask : public ilrd::scheduler::ISchedulerTask
    {
    public:
        AsyncInjectionTask(AsyncInjection& async_injection); 
        void Run(); 

    private:
        AsyncInjection& m_async_injection; 
    }; 
}; 
}

#endif //ILRD_RD1645_ASYNC_INJECTION_HPP