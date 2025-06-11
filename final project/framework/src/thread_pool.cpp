/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 10.3.25
Exercise: Thread Pool
***/
#include <semaphore> /* std::counting_semaphore */
#include <iostream> /* test */
#include <itp_task.hpp> /* API */
#include "thread_pool.hpp" /* API */

/*========================== Thread Pool Static API ========================*/
static const int s_admin_priority = static_cast<int>(ilrd::ThreadPool::HIGH) + 1; 
static const int s_admin_low_priority = static_cast<int>(ilrd::ThreadPool::LOW) - 1; 

thread_local bool ilrd::ThreadPool::s_is_running = true; 

void ilrd::ThreadPool::BadAppleFunction()
{
    s_is_running = false;  
}

void ilrd::ThreadPool::PauseFunction(ilrd::ThreadPool* tp)
{
    tp->m_semaphore_pause.release(); 
    tp->m_semaphore_resume.acquire(); 
}

/*============================== Helper Functions ============================*/

void ilrd::ThreadPool::ThreadWork()
{
    
    while(s_is_running)
    {
        ilrd::ThreadPool::InternalTask task;
        m_waitable_queue.Pop(task);
        (task.GetITPTask())->Run();
    }

    m_non_active_thread_queue.Push(std::this_thread::get_id()); 
    m_semaphore_exit.release(); 
}

void ilrd::ThreadPool::StopThreads(size_t no_threads, priority priority)
{
    size_t counting_threads = 0; 

    for (size_t i = 0; i < no_threads; ++i) 
    {
        AddTask(m_bad_apple_task, priority);

        if(m_pause)
        {
            m_semaphore_resume.release();
        }
    }

    while(counting_threads < no_threads)
    {
        m_semaphore_exit.acquire(); 
        ++counting_threads;
    }

    while (!m_non_active_thread_queue.Empty()) 
    {
        std::thread::id pop_id; 
        m_non_active_thread_queue.Pop(pop_id); 
        auto it = m_threads_map.find(pop_id); 
        (*(it->second)).join(); 
        m_threads_map.erase(it);
    }
}

/*============================== ThreadPool API ============================*/

ilrd::ThreadPool::ThreadPool(size_t threads_no) : m_threads_no(threads_no), m_pause(false),            
                m_bad_apple_task(new ilrd::thread_pool::FunctionTask<>(ilrd::ThreadPool::BadAppleFunction)),
                m_pause_task(new ilrd::thread_pool::FunctionTask<ThreadPool*>(ilrd::ThreadPool::PauseFunction, this)),
                m_semaphore_exit{0}, m_semaphore_pause{0}, m_semaphore_resume{0}
                
{
    for (size_t i = 0; i < m_threads_no; ++i) 
    {
        std::shared_ptr<std::thread> thread(new std::thread(&ilrd::ThreadPool::ThreadWork, this));
        m_threads_map.emplace(thread->get_id(), thread);
    }
}

ilrd::ThreadPool::~ThreadPool() noexcept
{
    StopThreads(m_threads_no, static_cast<priority>(s_admin_low_priority));
}

void ilrd::ThreadPool::AddTask(std::shared_ptr<ilrd::thread_pool::ITPTask> task_ptr, 
                                                    priority task_priotity)
{
    m_waitable_queue.Push((ilrd::ThreadPool::InternalTask(task_ptr, task_priotity))); 
}

void ilrd::ThreadPool::SetNum(size_t threads_no)
{
    int diff = threads_no - m_threads_no;

    if(diff >= 0)
    {
        for (int i = 0; i < diff; ++i) 
        {
            std::shared_ptr<std::thread> thread
                        (new std::thread(&ilrd::ThreadPool::ThreadWork, this));
            m_threads_map.emplace(thread->get_id(), thread);       
        } 
    }
    else
    {
        StopThreads(-diff, static_cast<priority>(s_admin_priority)); 
    }

    m_threads_no = threads_no; 
}

void ilrd::ThreadPool::Pause()
{
    size_t counting_threads = 0; 
    m_pause = true; 

    while(counting_threads < m_threads_no)
    {
        AddTask(m_pause_task, static_cast<priority>(s_admin_priority)); 
        ++counting_threads;
    }

    counting_threads = 0; 

    while(counting_threads < m_threads_no)
    {
        m_semaphore_pause.acquire(); 
        ++counting_threads;
    }
}

void ilrd::ThreadPool::Resume()
{
    m_pause = false; 
    size_t counting_threads = 0; 
    while(counting_threads < m_threads_no)
    {
        m_semaphore_resume.release(m_threads_no); 
        ++counting_threads;
    }
    
}

/*============================== InternalTask API ========================*/

ilrd::ThreadPool::InternalTask::InternalTask() 
{

}

ilrd::ThreadPool::InternalTask::InternalTask
        (std::shared_ptr<ilrd::thread_pool::ITPTask> task, priority priority) :
                                m_user_tasks(task), m_priority(priority)
{

}

bool ilrd::ThreadPool::InternalTask::operator<(const ilrd::ThreadPool::InternalTask& other) const
{
    return static_cast<int>(m_priority) < static_cast<int>(other.m_priority); 

}

ilrd::ThreadPool::priority ilrd::ThreadPool::InternalTask::GetPriority()
{
    return m_priority; 
}

void ilrd::ThreadPool::InternalTask::SetPriority(priority priority)
{
    m_priority = priority; 
}

std::shared_ptr<ilrd::thread_pool::ITPTask> ilrd::ThreadPool::InternalTask::GetITPTask()
{
    return m_user_tasks;
}