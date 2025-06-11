/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 16.3.25
Exercise: Singleton
***/

#ifndef ILRD_SINGLETON_HPP
#define ILRD_SINGLETON_HPP

#include <cstdlib>     /* atexit */
#include <atomic> /* std::atomic */

namespace ilrd
{

template <typename T>
class Singleton
{
public:
    // Singleton should not be copiable
    Singleton(const Singleton& other) = delete; 

    // Singleton should not be assignable
    Singleton& operator=(const Singleton& other) = delete; 

    static T* GetInstace(); 

private:

    // Singleton Ctor should be private to prevent direct construction calls
    Singleton(); 
    
    static std::atomic<T*> m_singelton;   
    static std::mutex m_mutex;  
    static void DeleteSingleton() noexcept; 
};


template<typename T> 
std::atomic<T*> ilrd::Singleton<T>::m_singelton = nullptr; 

template<class T> 
std::mutex ilrd::Singleton<T>::m_mutex;

template<typename T> 
ilrd::Singleton<T>::Singleton()                                                               
{}

template<typename T> 
T* ilrd::Singleton<T>::GetInstace()
{
    T* temp = m_singelton.load(std::memory_order_acquire) ; //insert memory barrier
    
    if(temp == NULL) //first check
    {
        std::lock_guard<std::mutex> lock(ilrd::Singleton<T>::m_mutex); //Lock
        temp = m_singelton.load(std::memory_order_relaxed); 
        if(temp == NULL) //second check
        {
            temp =  new T();  
            m_singelton.store(temp, std::memory_order_release); //insert memory barrier
            std::atexit(ilrd::Singleton<T>::DeleteSingleton); 
        }
    }

    return temp; 
}

template<typename T> 
void ilrd::Singleton<T>::DeleteSingleton() noexcept
{
    delete m_singelton;
    m_singelton = reinterpret_cast<T*>(0xDEADBEEF); 
}

} // namespace ilrd

#endif //ILRD_SINGLETON_HPP