/*** 
Written by: Shani Davidian
Reviewed by: Gil
Date: 17.3.25
Exercise: Handleton
***/

#include <iostream>

#include <handleton.hpp>
#include <thread_pool.hpp>

extern "C"
{
    void PluginHandle2(); 
}

void PluginHandle2()
{
    std::cout << "Object Address is: "; 
    std::cout << ilrd::Handleton<ilrd::ThreadPool>::GetInstace() << std::endl;
}