
/*** 
Written by: Shani Davidian
Reviewed by: Zack
Date: 25.3.25
Exercise: Scheduler
***/

//gdpp23 -shared -fPIC -o libshared_dll2.so -I./include src/thread_pool.cpp src/handleton.cpp src/scheduler.cpp
// gdpp23 test/test_scheduler.cpp -o test.out -I./include -Wl,-rpath,. -L. -lshared_dll2 
#include <iostream>
#include "scheduler.hpp"

using namespace ilrd; 
using namespace ilrd::scheduler; 

static void Task1(int num)
{
    std::cout << "I'm doing Task1" << std::endl;
    (void)sleep(10);
    std::cout << "I'm done with Task1" << std::endl;
}

static void Task2()
{
    std::cout << "I'm doing Task2" << std::endl;
    // (void)sleep(3);
    std::cout << "I'm done with Task2" << std::endl;
}

int main()
{
    Scheduler* scheduler = ilrd::Handleton<ilrd::Scheduler>::GetInstace(); 
    std::shared_ptr<SchedulerTask<int>> task1(new SchedulerTask<int>(Task1, 100));
    std::shared_ptr<SchedulerTask<>> task2(new SchedulerTask<>(Task2));

    scheduler->AddTask(task1, std::chrono::milliseconds(10));
    scheduler->AddTask(task2, std::chrono::milliseconds(30));
    scheduler->AddTask(task2, std::chrono::milliseconds(60));

    sleep(50);

    return 0; 
}