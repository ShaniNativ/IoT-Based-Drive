/*** 
Written by: Shani Davidian
Reviewed by: Liad
Date: 10.3.25
Exercise: Thread Pool
***/
// #include <unistd.h>
// #include <iostream>
// #include "thread_pool.hpp" /* API */
// #include "itp_task.hpp"
// #include "function_task.hpp"
// #include "future_task.hpp"

// void Thread_LOW()
// {
//     sleep(10);
//     std::cout << "LOW\n";
// }

// void Thread_MED()
// {
//     sleep(1);
//     std::cout << "MEDIUM\n";
// }

// void Thread_HIGH()
// {
//     std::cout << "HIGH\n";
// }

// int FutureFunc()
// {

//     return 1000; 
// }

// void PlainTask()
// {
//     sleep(1);
//     std::cout << "Plain Task\n";
// }

// void SpecialTask()
// {
//     sleep(1);
//     std::cout << "SpecialTask\n";
// }


// void Test1()
// {
//     ilrd::ThreadPool thread_pool(5); 
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task1(new ilrd::thread_pool::FunctionTask<>(Thread_LOW));
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task2(new ilrd::thread_pool::FunctionTask<>(Thread_MED));
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task3(new ilrd::thread_pool::FunctionTask<>(Thread_HIGH));
//     std::shared_ptr<ilrd::thread_pool::FutureTask<int>> task4(new ilrd::thread_pool::FutureTask<int>(FutureFunc));

//     for(size_t i = 0; i < 5; ++i)
//     {
//         thread_pool.AddTask(task1, ilrd::ThreadPool::LOW);
//     }

//     for(size_t i = 0; i < 5; ++i)
//     {
//         thread_pool.AddTask(task2, ilrd::ThreadPool::MEDIUM);
//     }

//     for(size_t i = 0; i < 5; ++i)
//     {
//         thread_pool.AddTask(task3, ilrd::ThreadPool::HIGH);
//     }

//     thread_pool.AddTask(task3, ilrd::ThreadPool::HIGH);
//     thread_pool.AddTask(task4, ilrd::ThreadPool::LOW);
//     std::cout << task4->Get() << std::endl; 
// }

// void Test2()
// {
//     ilrd::ThreadPool thread_pool(1); 
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task1(new ilrd::thread_pool::FunctionTask<>(PlainTask));

//     for(size_t i = 0; i < 100; ++i)
//     {
//         thread_pool.AddTask(task1, ilrd::ThreadPool::LOW);
//     }
//     sleep(5);
//     thread_pool.SetNum(8);
//     thread_pool.Pause();
//     sleep(3);
   
//     thread_pool.Resume();
    
//     thread_pool.SetNum(4);
// }

// void Test3()
// {
//     ilrd::ThreadPool thread_pool(8); 
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task1(new ilrd::thread_pool::FunctionTask<>(PlainTask));
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task2(new ilrd::thread_pool::FunctionTask<>(SpecialTask));
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task3(new ilrd::thread_pool::FunctionTask<>(Thread_HIGH));
//     std::shared_ptr<ilrd::thread_pool::ITPTask> task4(new ilrd::thread_pool::FunctionTask<>(Thread_MED));
//     std::cout << "==================== TEST 3 ===============\n";


//     for(size_t i = 0; i < 30; ++i)
//     {
//         thread_pool.AddTask(task1, ilrd::ThreadPool::LOW);
//         thread_pool.AddTask(task3, ilrd::ThreadPool::HIGH);
//         thread_pool.AddTask(task4, ilrd::ThreadPool::MEDIUM);
//     }

//     sleep(3);
//     thread_pool.Pause();
//     sleep(1);
//     std::cout << "==================== PAUSED ===============\n";
//     thread_pool.SetNum(1);
//     sleep(3);
//     std::cout << "==================== SET TO 1 THREAD ===============\n";
//     thread_pool.AddTask(task2, ilrd::ThreadPool::HIGH);
//     std::cout << "==================== ADD TASK ===============\n";
//     thread_pool.AddTask(task2, ilrd::ThreadPool::HIGH);
//     std::cout << "==================== ADD TASK ===============\n";
//     sleep(1);
//     thread_pool.Resume();
//     sleep(2);

//     std::cout << "==================== RESUMED ===============\n";
//     thread_pool.SetNum(4);
//     std::cout << "==================== SET TO 4 THREAD ===============\n";

// }

// void Test4()
// {

//     ilrd::ThreadPool thread_pool(1); 
//     sleep(2);
//     std::cout << "==================== TEST 4 ===============\n";
//     thread_pool.Pause();
//     std::cout << "==================== PAUSED ===============\n";
//     thread_pool.Resume();

// }

// int main()
// {

//     // Test1();
//     // Test2();
//     Test3();
//     Test4();


//     return 0;
// }





// Amit's Test
// Filename: ThreadPoolTest.cpp
// Created by shiningblack on 3/9/25.
// Reviewer:
// Review Status:
// Filename: ThreadPoolTest.cpp
// Created by shiningblack on 3/9/25.
// Reviewer:
// Review Status:

#include <iostream>
#include <random>

#include "test_macros.h"
#include "thread_pool.hpp"
#include "itp_task.hpp"

static int Task1(const int num)
{
    (void)sleep(2U);
    return num;
}

static void Task2()
{
    std::cout << "I'm doing Task2" << std::endl;
    (void)sleep(3U);
    std::cout << "I'm done with Task2" << std::endl;
}

void BasicTest()
{
    // ilrd::ThreadPool pool(4U);
    // int resArr[8U] = {};
    // std::shared_ptr<ilrd::thread_pool::FutureTask<int, int>> futures[8U];
    // for (size_t i = 0U; i < 8U; ++i)
    // {
    //     FutureTask[i] = std::make_shared<ilrd::thread_pool::FutureTask<int>>(Task1, 1);
    // }
    // for (const auto & future : futures)
    // {
    //     pool.AddTask(future, ilrd::ThreadPool::priority::LOW);
    // }
    // for (const auto & future : futures)
    // {
    //     std::cout << future->Get() << std::endl;
    // }
}

void TestPauseResume()
{
    ilrd::ThreadPool pool(4U);
    const auto task = std::make_shared<ilrd::thread_pool::FunctionTask<>>(Task2);
    std::cout << "Start Pause Resume" << std::endl;

    (void)pool.Pause();

    for (size_t i = 0U; i < 8U; ++i)
    {
        pool.AddTask(task, ilrd::ThreadPool::priority::LOW);
    }

    (void)sleep(3U);

    (void)pool.Resume();

    //(void)pool.Pause();
}

void TestResize()
{
    ilrd::ThreadPool pool(4U);
    const auto task = std::make_shared<ilrd::thread_pool::FunctionTask<>>(Task2);
    std::cout << "Start RESIZE test" << std::endl;

    for (size_t i = 0U; i < 20U; ++i)
    {
        pool.AddTask(task, ilrd::ThreadPool::priority::LOW);
    }

    pool.SetNum(2U);

    (void)sleep(10U);

    for (size_t i = 0U; i < 40U; ++i)
    {
        pool.AddTask(task, ilrd::ThreadPool::priority::LOW);
    }

    (void)pool.Pause();
    std::cout << "PAUSED" << std::endl;
    pool.SetNum(6U);

    (void)sleep(10U);
    (void)pool.Resume();
}


void EvilTest()
{
    ilrd::ThreadPool pool(0U);
    int num;
    const auto task = std::make_shared<ilrd::thread_pool::FunctionTask<>>(Task2);
    const auto task2 = std::make_shared<ilrd::thread_pool::FutureTask<int, int>>(Task1, 1);
    std::cout << RED"EVIL LAIR BOSS STAGE" << std::endl;

    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Resume();

    pool.SetNum(1U);
    (void)pool.Pause();
    pool.SetNum(5U);
    pool.SetNum(2U);
    pool.SetNum(99U);
    pool.SetNum(4U);
    (void)pool.Resume();
    (void)pool.Resume();
    (void)pool.Resume();
    (void)pool.Pause();
    (void)pool.Pause();
    (void)pool.Resume();

    for (size_t i = 0U; i < 20U; ++i)
    {
        pool.AddTask(task, ilrd::ThreadPool::priority::LOW);
    }

    (void)pool.Pause();
    pool.SetNum(2U);
    (void)pool.Resume();
    pool.AddTask(task2, ilrd::ThreadPool::priority::HIGH);
    pool.AddTask(task2, ilrd::ThreadPool::priority::LOW);
    num = task2->Get();
    std::cout << "Got result" << std::endl;
    --num;
    pool.SetNum(4U);
    (void)task2->Get();
    std::cout << "Got result" << std::endl;

    for (size_t i = 0U; i < 10U; ++i)
    {
        pool.AddTask(task, ilrd::ThreadPool::priority::LOW);
    }

    (void)pool.Pause();
    pool.SetNum(0U);
    (void)pool.Resume();
    pool.SetNum(5U);
    pool.SetNum(5U);
    (void)pool.Pause();
    std::cout << GREEN"Le finale" << std::endl;
}

int main()
{
    BasicTest();
    TestPauseResume();
    TestResize();
    EvilTest();
    PASS;
    return 0;
}
