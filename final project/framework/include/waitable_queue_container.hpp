/*** 
Written by: Shani Davidian
Reviewed by: Ofir W.
Date: 9.3.25
Exercise: Waitable Queue
***/

#ifndef ILRD_RD1645_WAITABLE_QUEUE_CONTAINER_HPP
#define ILRD_RD1645_WAITABLE_QUEUE_CONTAINER_HPP

#include <queue> /* std::queue, std::priority_queue */

namespace ilrd
{
    // T element must be copiable and assigment
    template <typename T, typename Container = std::queue<T>>
    class WaitableQueueContainer
    {
    public:
        WaitableQueueContainer(const WaitableQueueContainer& other) = delete;
        WaitableQueueContainer& operator=(const WaitableQueueContainer& other) = delete;
        void Push(const T& data);
        void Pop(T& out); 
        bool Empty() const; 

    protected:
        WaitableQueueContainer() = default;
        ~WaitableQueueContainer() = default;
        
    private:
        Container m_queue;
    };

    template <typename T> //template partial specialization 
    class WaitableQueueContainer<T, std::priority_queue<T>>
    {
    public:
        WaitableQueueContainer(const WaitableQueueContainer& other) = delete;
        WaitableQueueContainer& operator=(const WaitableQueueContainer& other) = delete;
        void Push(const T& data);
        void Pop(T& out); 
        bool Empty() const; 

    protected:
        WaitableQueueContainer() = default;
        ~WaitableQueueContainer() = default;
        
    private:
        std::priority_queue<T> m_queue; 
    };
}

/*===================== WaitableQueueContainer Template Implementation ================*/

template<class T, class Container>
void ilrd::WaitableQueueContainer<T, Container>::Push(const T& data)
{      
    m_queue.push(data); 
}

template<class T, class Container> 
void ilrd::WaitableQueueContainer<T, Container>::Pop(T& out)
{
    out = m_queue.front();
    m_queue.pop(); 
}

template<class T, class Container> 
bool ilrd::WaitableQueueContainer<T, Container>::Empty() const 
{
    return m_queue.empty(); 
}

/*================= WaitableQueueContainer Template Partial Specialization =============*/

template<class T> 
void ilrd::WaitableQueueContainer<T, std::priority_queue<T>>::Push(const T &data)
{
    m_queue.push(data); 
}



template<class T> 
void ilrd::WaitableQueueContainer<T, std::priority_queue<T>>::Pop(T& out)
{
    out = m_queue.top();
    m_queue.pop(); 
}


template<class T> 
bool ilrd::WaitableQueueContainer<T, std::priority_queue<T>>::Empty() const
{
    return m_queue.empty(); 
}


#endif //ILRD_RD1645_WAITABLE_QUEUE_CONTAINER_HPP