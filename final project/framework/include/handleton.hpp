/*** 
Written by: Shani Davidian
Reviewed by: Gil
Date: 17.3.25
Exercise: Handleton
***/

#ifndef ILRD_HANDLETON_HPP
#define ILRD_HANDLETON_HPP

#include <tuple> /* std::tuple */
#include <cstdlib>     /* atexit */
#include <functional> /* std::ref */
#include <atomic> /* std::atomic */

namespace ilrd
{

#ifdef I_AM_THE_REAL_HANDLETON_IMP

template <typename T>
class Handleton
{
public:
    // Handleton should not be copiable
    Handleton(const Handleton& other) = delete; 

    // Handleton should not be assignable
    Handleton& operator=(const Handleton& other) = delete; 

    static T* GetInstace(); 

private:

    // Handleton Ctor should be private to prevent direct construction calls
    Handleton(); 
    
    static std::atomic<T*> m_singelton;   
    static std::mutex m_mutex;  
    static void DeleteHandleton() noexcept; 
};

template<typename T> 
std::atomic<T*> ilrd::Handleton<T>::m_singelton = nullptr; 

template<class T> 
std::mutex ilrd::Handleton<T>::m_mutex;

template<typename T> 
ilrd::Handleton<T>::Handleton()                                                               
{}

template<typename T> 
T* ilrd::Handleton<T>::GetInstace()
{
    T* temp = m_singelton.load(std::memory_order_acquire) ; //insert memory barrier
    
    if(temp == NULL) //first check
    {
        std::lock_guard<std::mutex> lock(ilrd::Handleton<T>::m_mutex); //Lock
        temp = m_singelton.load(std::memory_order_relaxed); 
        if(temp == NULL) //second check
        {
            temp =  new T();  
            m_singelton.store(temp, std::memory_order_release); //insert memory barrier
            std::atexit(ilrd::Handleton<T>::DeleteHandleton); 
        }
    }

    return temp; 
}

template<typename T> 
void ilrd::Handleton<T>::DeleteHandleton() noexcept
{
    delete m_singelton;
    m_singelton = reinterpret_cast<T*>(0xDEADBEEF); 
}

#else

template <typename T>
class Handleton
{
public:
    // Handleton should not be copiable
    Handleton(const Handleton& other) = delete; 

    // Handleton should not be assignable
    Handleton& operator=(const Handleton& other) = delete; 

    static T* GetInstace(); 

private:

    // Handleton Ctor should be private to prevent direct construction calls
    Handleton(); 
    
    static std::atomic<T*> m_singelton;   
    static std::mutex m_mutex;  
    static void DeleteHandleton() noexcept; 
};

#endif //I_AM_THE_REAL_HANDLETON_IMP

} // namespace ilrd

#endif //ILRD_HANDLETON_HPP