/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 25.3.25
Exercise: Async-Injection
***/

#include <iostream>
#include "async_injection.hpp"

static int g_counter = 10; 

bool Task1()
{
    std::cout << "Task1 - no repeat" << std::endl; 
    return 0; 
}

bool Task2()
{
    std::cout << "Task1 - repeat" << std::endl; 
    --g_counter; 
    
    return g_counter; 
}

int main()
{

    new ilrd::AsyncInjection(Task2, std::chrono::milliseconds(1000)); 

    sleep(20);

    return 0; 

}