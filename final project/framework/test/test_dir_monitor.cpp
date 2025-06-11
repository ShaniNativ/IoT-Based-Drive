/*** 
Written by: Shani Davidian
Reviewed by: Amit
Date: 18.3.25
Exercise: Dir Monitor
***/

#include <dlfcn.h> /* dlopen */
#include <unistd.h>
#include <iostream>

#include "dir_monitor.hpp"
#include "dll_loader.hpp"

int main()
{
    ilrd::DLLloader loader;
    ilrd::DirMonitor monitor("/home/shanid/shani.davidian/projects/final_project/framework/test_dll_monitor");
    ilrd::Callback<ilrd::DLLloader,const std::string&> callback_load(loader, &ilrd::DLLloader::Load);
    ilrd::Callback<ilrd::DLLloader, const std::string&> callback_unload(loader, &ilrd::DLLloader::UnLoad);
    monitor.RegisterLoad(&callback_load);
    monitor.RegisterUnLoad(&callback_unload);
    monitor.Run(); 

    sleep(20); 
    std::cout << "finish";
    return 0; 
}