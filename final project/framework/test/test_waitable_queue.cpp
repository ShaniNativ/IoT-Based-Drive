/*** 
Written by: Shani Davidian
Reviewed by: Ofir W.
Date: 9.3.25
Exercise: Waitable Queue
***/

#include <thread>
#include <iostream>
#include "waitable_queue.hpp"

ilrd::WaitableQueue<int> my_queue; 
static int g_atomic = 0; 
static int g_size = 200;
static int g_readers = 2;
//static int g_writers = 2;


void PushThread()
{
    size_t counter = 0; 
    while(g_atomic < g_size)
    {
        my_queue.Push(++g_atomic); 
       // std::cout << "Write:" << g_atomic << std::endl; 
        ++counter;
    }
}

void PopThread()
{
    int counter = 0; 
    int temp; 
    while(counter < g_size/g_readers)
    {
        my_queue.Pop(temp);
        std::cout << "Read:" << temp << std::endl; 
        ++counter;
    }
}

void TestPop()
{
    std::thread push_thread_1(PushThread); 
    std::thread push_thread_2(PushThread); 

    

    std::thread pop_thread1(PopThread); 
    std::thread pop_thread2(PopThread); 

    push_thread_1.join(); 
    push_thread_2.join(); 
   
    pop_thread1.join();
    pop_thread2.join();
}

void TestPopTimeout()
{
    int temp;
    if(0 == my_queue.Pop(temp, std::chrono::milliseconds(4000)))
    {
        std::cout << "Timeout" << std::endl; 
    }
}

int main()
{
    TestPopTimeout();
    TestPop();
    

    return 0;
}