/*** 
Written by: Shani Davidian
Reviewed by: Zack
Date: 25.3.25
Exercise: Scheduler
***/

#ifndef ILRD_RD1645_SCHEDULER_TASK_HPP
#define ILRD_RD1645_SCHEDULER_TASK_HPP

#include <functional> /* std::function */
#include <tuple> /* std::tuple */

namespace ilrd
{

namespace scheduler
{

class ISchedulerTask
{
public:
    virtual void Run() = 0; 
};

template <typename... ARGS>
class SchedulerTask : public ISchedulerTask
{
public:
    SchedulerTask(std::function<void(ARGS...)> function, ARGS... args);
    void Run(); 

private:
    std::function<void(ARGS...)> m_function;
    std::tuple<ARGS...> m_args;
};
        
}//scheduler

}//ilrd

/*=============================== Scheduler Task ===============================*/

template<class... ARGS> 
ilrd::scheduler::SchedulerTask<ARGS...>::SchedulerTask(std::function<void (ARGS...)> function, ARGS ...args) :
                m_function(function), m_args(std::make_tuple(std::ref(args)...))
{

}

template<class... ARGS> 
void ilrd::scheduler::SchedulerTask<ARGS...>::Run()
{
    std::apply(m_function, m_args); 
}


#endif //ILRD_RD1645_SCHEDULER_TASK_HPP