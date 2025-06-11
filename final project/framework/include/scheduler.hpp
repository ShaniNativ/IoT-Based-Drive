
/*** 
Written by: Shani Davidian
Reviewed by: Zack
Date: 25.3.25
Exercise: Scheduler
***/

#ifndef ILRD_RD1645_SCHEDULER_HPP
#define ILRD_RD1645_SCHEDULER_HPP

#include <time.h> /* time_t */
#include <signal.h> /* struct sigevent */
#include <mutex> /* std::mutex */
#include <queue> /* std::priority_queue */
#include <memory> /* shared_ptr */

#include "scheduler_task.hpp"
#include "handleton.hpp"

namespace ilrd
{

class Scheduler
{
    class WrapperTask;
public:  
    //notice --- if the dtor is called when queue still has tasks, those task will be destroyed and not run!
    ~Scheduler(); 
     //notice --- tasks will not run in parrallel!
    void AddTask(std::shared_ptr<ilrd::scheduler::ISchedulerTask> task, 
                                            std::chrono::milliseconds ms);
    friend class ilrd::Handleton<Scheduler>; 

private:
    Scheduler(); 
    std::priority_queue<ilrd::Scheduler::WrapperTask> m_tasks_queue; 
    static void TimerRun(union sigval sigev_value); 
    void ResetTimer();
    void TimerRunMethod();
    timer_t m_timerid;
    struct sigevent m_evp; 
    mutable std::mutex m_timer_mutex; 
    mutable std::mutex m_pqueue_mutex; 

    class WrapperTask
    {
    public:
        WrapperTask() = default; 
        WrapperTask(std::shared_ptr<ilrd::scheduler::ISchedulerTask> task , 
                        std::chrono::time_point<std::chrono::high_resolution_clock> ms); 
        bool operator<(const WrapperTask& other) const; 
        bool operator==(const WrapperTask& other) const; 
        std::shared_ptr<ilrd::scheduler::ISchedulerTask> GetUserTask() const; 
        std::chrono::time_point<std::chrono::high_resolution_clock> GetTaskTime() const; 
    private:
        std::shared_ptr<ilrd::scheduler::ISchedulerTask> m_user_task;
        std::chrono::time_point<std::chrono::high_resolution_clock>  m_ms; 
    }; 
};

}

#endif
