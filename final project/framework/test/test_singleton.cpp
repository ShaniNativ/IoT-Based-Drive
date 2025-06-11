/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 16.3.25
Exercise: Singleton
***/

#include <iostream>
#include <thread>
#include "singleton.hpp"

using namespace ilrd; 
class Company
{
public:
    void Print()
    {
        std::cout << "Company class" << std::endl;
    }
};

void ThreadHandle()
{
    Singleton<Company>::GetInstace()->Print(); /* why this line creat two allocs ?? in valgrind*/
    Singleton<Company>::GetInstace()->Print();
}
int main()
{
    Singleton<Company>::GetInstace()->Print();
    std::thread  thread1(ThreadHandle); 
    std::thread  thread2(ThreadHandle); 
    std::thread  thread3(ThreadHandle); 

    thread1.join(); 
    thread2.join();
    thread3.join();


    return 0; 
}