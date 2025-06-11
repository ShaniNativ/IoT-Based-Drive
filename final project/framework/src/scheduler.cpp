
/*** 
Written by: Shani Davidian
Reviewed by: Zack
Date: 25.3.25
Exercise: Scheduler
***/

#include "scheduler.hpp"

/*==================== Scheduler Class Implementation ==========================*/


void ilrd::Scheduler::ResetTimer()
{
    ilrd::Scheduler::WrapperTask task;

    {
        std::unique_lock<std::mutex> lock(m_pqueue_mutex); 
        task = m_tasks_queue.top(); 
    }
    auto time = task.GetTaskTime();
    auto sec = std::chrono::high_resolution_clock::to_time_t(time);
    auto ns = (time - std::chrono::high_resolution_clock::from_time_t(sec)).count();

    struct itimerspec value = {{0, 0}, {0, 0}}; 
    value.it_value.tv_sec =  sec;  
    value.it_value.tv_nsec = ns; 

    {
        std::unique_lock<std::mutex> lock(m_timer_mutex);

        if(-1 == timer_settime(m_timerid, TIMER_ABSTIME, &value, NULL))
        {
            throw std::runtime_error("Failed to reset timer"); 
        }
    }
}

void ilrd::Scheduler::TimerRun(sigval sigev_value)
{
    Scheduler* scheduler_class = static_cast<Scheduler*>(sigev_value.sival_ptr);
    scheduler_class->TimerRunMethod();
}

void ilrd::Scheduler::TimerRunMethod()
{
    ilrd::Scheduler::WrapperTask task;
    {
        std::unique_lock<std::mutex> lock(m_pqueue_mutex); 
        task = m_tasks_queue.top(); 
        m_tasks_queue.pop(); 
    }

    (task.GetUserTask())->Run(); 

    if(!m_tasks_queue.empty())
    {
        ResetTimer();
    }
}

void ilrd::Scheduler::AddTask(std::shared_ptr<ilrd::scheduler::ISchedulerTask> task, 
                                std::chrono::milliseconds ms) 
{
    ilrd::Scheduler::WrapperTask  new_task(task, std::chrono::high_resolution_clock::now() + ms);
    ilrd::Scheduler::WrapperTask  temp_task; 

    {
        std::unique_lock<std::mutex> lock(m_pqueue_mutex);
        m_tasks_queue.push(new_task);
        temp_task = m_tasks_queue.top();
    }

    if(temp_task == new_task)
    {
        ResetTimer();
    }
}

ilrd::Scheduler::Scheduler()
{
    m_evp.sigev_notify = SIGEV_THREAD;
    m_evp.sigev_value.sival_ptr = this;
    m_evp.sigev_notify_function = &TimerRun;
    m_evp.sigev_notify_attributes = 0;

    if(-1 == timer_create(CLOCK_REALTIME, &m_evp, &m_timerid))
    {
        throw std::runtime_error("Failed to create timer"); 
    }
}

ilrd::Scheduler::~Scheduler()
{
    timer_delete(m_timerid);
}

/*==================== Wrapper Class Implementation ==========================*/

ilrd::Scheduler::WrapperTask::WrapperTask
                (std::shared_ptr<ilrd::scheduler::ISchedulerTask> task, 
                std::chrono::time_point<std::chrono::high_resolution_clock> ms) : 
                m_user_task(task), m_ms(ms)
{

}

bool ilrd::Scheduler::WrapperTask::operator<
                            (const ilrd::Scheduler::WrapperTask& other) const
{
    return (m_ms > other.m_ms);
}


std::shared_ptr<ilrd::scheduler::ISchedulerTask> ilrd::Scheduler::WrapperTask::GetUserTask() const 
{
    return m_user_task; 
}

std::chrono::time_point<std::chrono::high_resolution_clock>  ilrd::Scheduler::WrapperTask::GetTaskTime() const
{
    return m_ms;
}

bool ilrd::Scheduler::WrapperTask::operator==(const ilrd::Scheduler::WrapperTask &other) const
{
    return (m_user_task == other.m_user_task) && (m_ms == other.m_ms);
}