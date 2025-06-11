/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/
#include <cstring>

#include "file_manager.hpp"
using namespace ilrd; 

int main()
{
    FileManager fm; 

    std::shared_ptr<char []> buffer(new char[20]); 
    memcpy(buffer.get(), "hello hello", 11); 

    std::shared_ptr<char []> buffer2(new char[11]); 

    fm.Write(11, 0, buffer); 

    fm.Read(11, 0, buffer2); 

    fm.Write(11, 12, buffer2); 

    std::shared_ptr<char []> buffer3(new char[25]); 
    memset(buffer3.get(), 90, 25);
    fm.Write(25, 0, buffer3); 

    return 0; 
}