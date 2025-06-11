/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Logger
***/

//#include "handleton.hpp"
#include "logger.hpp"

int main()
{
    //old test - now the logger is handleton!
    //ilrd::Logger my_log("test.txt"); 
    // my_log.Log("testttt", ilrd::Logger::DEBUG);
    // my_log.Log("fdgdgdfg", ilrd::Logger::WARNING);
    // my_log.Log("error!!!", ilrd::Logger::ERROR);

    //using handleton

    // ilrd::Handleton<ilrd::Logger>::GetInstace()->Log("first operation", ilrd::Logger::DEBUG);
    // ilrd::Handleton<ilrd::Logger>::GetInstace()->Log("second operation", ilrd::Logger::WARNING);
    // ilrd::Handleton<ilrd::Logger>::GetInstace()->Log("third operation", ilrd::Logger::ERROR);



    return 0;

}