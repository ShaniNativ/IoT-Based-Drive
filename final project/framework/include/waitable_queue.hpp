/*** 
Written by: Shani Davidian
Reviewed by: Ofir W.
Date: 9.3.25
Exercise: Waitable Queue
***/

#ifndef ILRD_RD1645_WAITABLE_QUEUE_HPP
#define ILRD_RD1645_WAITABLE_QUEUE_HPP

#include <mutex> /* std::mutex */
#include <condition_variable> /* std::condition_variable */
#include <chrono>

#include "waitable_queue_container.hpp" /* API */

namespace ilrd
{
    // T element must be copiable and assigment
    template <typename T, typename Container = std::queue<T>>
    class WaitableQueue : private WaitableQueueContainer<T, Container>
    {
    public:
        void Push(const T& data);
        bool Pop(T& out, std::chrono::milliseconds timeout); 
        void Pop(T& out); 
        bool Empty() const; 
    
    private:
        mutable std::timed_mutex m_mutex; 
        std::condition_variable_any m_cv; 
    };
}

/*===================== WaitableQueue Template Implementation ================*/

template<class T, class Container>
void ilrd::WaitableQueue<T, Container>::Push(const T& data)
{
    {
        std::unique_lock<std::timed_mutex> lock(m_mutex); 
        ilrd::WaitableQueueContainer<T, Container>::Push(data); 
    }

    m_cv.notify_one();  
}

template<class T, class Container> 
bool ilrd::WaitableQueue<T, Container>::Pop(T& out, std::chrono::milliseconds timeout)
{ 
    auto t0 = std::chrono::high_resolution_clock::now(); 
    std::unique_lock<std::timed_mutex> lock(m_mutex, timeout); 
    auto t1 = std::chrono::high_resolution_clock::now();

    auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
 
    timeout -= std::chrono::milliseconds(time_taken); 

    if(timeout.count() <= 0)
    {
        return false; 
    }

    if(ilrd::WaitableQueueContainer<T, Container>::Empty()) 
    {
        if(!m_cv.wait_for(lock, timeout, [this]{ return !ilrd::WaitableQueueContainer<T, Container>::Empty(); }))
        {
            return false; 
        }
    }
        
    ilrd::WaitableQueueContainer<T, Container>::Pop(out); 
    
    return true; 
}

template<class T, class Container> 
void ilrd::WaitableQueue<T, Container>::Pop(T& out)
{
    std::unique_lock<std::timed_mutex> lock(m_mutex); 

    if(ilrd::WaitableQueueContainer<T, Container>::Empty())
    {
        m_cv.wait(lock, [this]{ return !ilrd::WaitableQueueContainer<T, Container>::Empty(); });
    }
        
    ilrd::WaitableQueueContainer<T, Container>::Pop(out); 
}

template<class T, class Container> 
bool ilrd::WaitableQueue<T, Container>::Empty() const 
{
    std::unique_lock<std::timed_mutex> lock(m_mutex); 

    return ilrd::WaitableQueueContainer<T, Container>::Empty(); 
}

#endif //ILRD_RD1645_WAITABLE_QUEUE_HPP