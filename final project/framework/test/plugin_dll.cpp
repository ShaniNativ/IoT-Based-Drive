/*** 
Written by: Shani Davidian
Reviewed by: Gil
Date: 17.3.25
Exercise: Handleton
***/

#include <iostream>

//#define I_AM_THE_REAL_HANDLETON_IMP //handleton implementation
#include <handleton.hpp>
#include <thread_pool.hpp>

extern "C"
{
    void PluginHandle(); 
}

void PluginHandle()
{
    std::cout << "Object Address is: "; 
    std::cout << ilrd::Handleton<ilrd::ThreadPool>::GetInstace() << std::endl;
}