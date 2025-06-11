/*** 
Written by: Shani Davidian
Reviewed by: Idan
Date: 25.3.25
Exercise: Async-Injection
***/

#include "async_injection.hpp"

ilrd::AsyncInjection::AsyncInjection
        (std::function<bool ()> function, std::chrono::milliseconds interval) : 
        m_function(function), m_interval(interval), 
        m_async_injection_task(new ilrd::AsyncInjection::AsyncInjectionTask(*this))
{
    ilrd::Handleton<ilrd::Scheduler>::GetInstace()->AddTask(m_async_injection_task, m_interval); 
}


void ilrd::AsyncInjection::PerformAction()
{
    if(m_function())
    {
        ilrd::Handleton<ilrd::Scheduler>::GetInstace()->AddTask(m_async_injection_task, m_interval); 
    }
    else
    {
        delete this; 
    }
}

ilrd::AsyncInjection::AsyncInjectionTask::AsyncInjectionTask
                                        (ilrd::AsyncInjection& async_injection) : 
                                        m_async_injection(async_injection)
{

}

void ilrd::AsyncInjection::AsyncInjectionTask::Run()
{
    m_async_injection.PerformAction(); 
}
