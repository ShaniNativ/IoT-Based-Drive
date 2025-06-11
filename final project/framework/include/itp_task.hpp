/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 10.3.25
Exercise: Thread Pool
***/

#ifndef ILRD_RD1645_ITP_TASK_HPP
#define ILRD_RD1645_ITP_TASK_HPP

#if __cplusplus < 201103L
#  define noexcept throw()
#endif

#include <functional> /* std::function */
#include <semaphore> /* std::binary_semaphore */
#include <tuple>

namespace ilrd
{

namespace thread_pool
{

class ITPTask
{
public:
    virtual void Run() = 0; 
};

template <typename... ARGS>
class FunctionTask : public ITPTask
{
public:
    FunctionTask(std::function<void(ARGS...)> function, ARGS... args);
    void Run(); 
private:
    std::function<void(ARGS...)> m_function;
    std::tuple<ARGS...> m_args;
};


template<typename OUTPUT, typename... ARGS>
class FutureTask : public ITPTask
{
public:
    FutureTask(std::function<OUTPUT(ARGS...)> function, ARGS... args);
    void Run(); 
    OUTPUT Get(); //Blocking
private:
    std::function<OUTPUT(ARGS...)> m_function;
    std::binary_semaphore m_sem;
    OUTPUT m_output; 
    std::tuple<ARGS...> m_args;
};

}//thread_pool

}//ilrd

/*=============================== Function Task ===============================*/

template<class... ARGS> 
ilrd::thread_pool::FunctionTask<ARGS...>::FunctionTask
            (std::function<void (ARGS...)> function, ARGS ...args) :
            m_function(function), m_args(std::make_tuple(std::ref(args)...))
{

}

template<class... ARGS> 
void ilrd::thread_pool::FunctionTask<ARGS...>::Run()
{
    std::apply(m_function, m_args); 
}

/*=============================== Future Task =================================*/

template<class OUTPUT, class... ARGS> 
ilrd::thread_pool::FutureTask<OUTPUT, ARGS...>::
            FutureTask(std::function<OUTPUT (ARGS...)> function, ARGS... args) : 
            m_function(function), m_sem{0}, m_args(std::make_tuple(std::ref(args)...))
{

}

template<class OUTPUT, class... ARGS> 
void ilrd::thread_pool::FutureTask<OUTPUT, ARGS...>::Run()
{
    m_output = std::apply(m_function, m_args); 
    m_sem.release(); 
}

template<class OUTPUT, class... ARGS> 
OUTPUT ilrd::thread_pool::FutureTask<OUTPUT, ARGS...>::Get()
{
    m_sem.acquire(); 
    return m_output; 
}


#endif //ILRD_RD1645_ITP_TASK_HPP