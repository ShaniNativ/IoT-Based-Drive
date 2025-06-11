/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 10.3.25
Exercise: Thread Pool
***/

#ifndef ILRD_RD1645_THREAD_POOL_HPP
#define ILRD_RD1645_THREAD_POOL_HPP

#if __cplusplus < 201103L
#  define noexcept throw()
#endif

#include <memory> /* shared_ptr */
#include <map> /* std::map */
#include <mutex> /* std::mutex */
#include <thread> /* thread */
#include <atomic> /* atomic_bool */

#include "itp_task.hpp" /* API */
#include "waitable_queue.hpp" /* API */
#include "handleton.hpp" /* API */

namespace ilrd
{

class ThreadPool
{
    class InternalTask;
public:
    //any diffrent task priority will cause undefined behavior
    enum priority{LOW, MEDIUM, HIGH}; 

    ~ThreadPool() noexcept; //blocking

    void AddTask(std::shared_ptr<ilrd::thread_pool::ITPTask> task_ptr, priority task_priotity = LOW);

    //SetNun Will not work when the pool is in Pause state
    void SetNum(size_t threads_no); //blocking

    void Pause(); //blocking

    void Resume(); 
    friend class ilrd::Handleton<ThreadPool>; 
private:
    explicit ThreadPool(size_t threads_no = std::thread::hardware_concurrency() - 1);
    size_t m_threads_no; 
    std::atomic_bool m_pause;
    std::shared_ptr<ilrd::thread_pool::ITPTask> m_bad_apple_task; 
    std::shared_ptr<ilrd::thread_pool::ITPTask> m_pause_task; 
    std::mutex m_mutex;
    std::counting_semaphore<INT_MAX> m_semaphore_exit; 
    std::counting_semaphore<INT_MAX> m_semaphore_pause; 
    std::counting_semaphore<INT_MAX> m_semaphore_resume; 
    thread_local static bool s_is_running;
    WaitableQueue<InternalTask, std::priority_queue<InternalTask>> m_waitable_queue;
    std::map<std::thread::id, std::shared_ptr<std::thread>> m_threads_map; 
    WaitableQueue<std::thread::id> m_non_active_thread_queue;
    void ThreadWork();
    void StopThreads(size_t no_threads, priority priority); 
    static void BadAppleFunction(); 
    static void PauseFunction(ThreadPool* tp);
    
    class InternalTask
    {
    public:
        InternalTask(); 
        InternalTask(std::shared_ptr<ilrd::thread_pool::ITPTask> task, priority priority); 
        bool operator<(const InternalTask& other) const; 
        priority GetPriority(); 
        void SetPriority(priority priority); 
        std::shared_ptr<ilrd::thread_pool::ITPTask> GetITPTask();
    private:
        std::shared_ptr<ilrd::thread_pool::ITPTask> m_user_tasks; 
        priority m_priority; 
    };
};

} // namespace ilrd

#endif //ILRD_RD1645_THREAD_POOL_HPP
