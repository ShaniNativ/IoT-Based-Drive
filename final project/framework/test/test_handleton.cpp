/*** 
Written by: Shani Davidian
Reviewed by: Gil
Date: 17.3.25
Exercise: Handleton
***/

//Linking 
// gdpp23 -fPIC -I./include -c src/handleton.cpp -o handleton.o
// gdpp23 -fPIC -I./include -c src/thread_pool.cpp -o thread_pool.o
// gdpp23 -shared -o libshared_dll.so -I./include  thread_pool.o handleton.o
// gdpp23 -fPIC -shared test/plugin_dll.cpp -o plugin_dll.so -I./include -Wl,-rpath,. -L. -lshared_dll
// gdpp23 test/test_handleton.cpp -o test.out -I./include -Wl,-rpath,. -L. -lshared_dll  

#include <iostream>
#include <dlfcn.h>
//#define I_AM_THE_REAL_HANDLETON_IMP //handleton implementation
#include <handleton.hpp>
#include <thread_pool.hpp>

int main()
{
    void (*sym_handle)() ;
    void (*sym_handle2)() ;
    std::cout << "Direct CALL : Object Address is: "; 
    std::cout << ilrd::Handleton<ilrd::ThreadPool>::GetInstace() << std::endl;

    void* handle = dlopen("./plugin_dll.so",RTLD_NOW);
    std::cout << "plugin_dll.so : "; 
    *(void**)(&sym_handle) = dlsym(handle, "PluginHandle");

    void* handle2 = dlopen("./plugin_dll2.so",RTLD_NOW);
    std::cout << "plugin_dll2.so : "; 
    *(void**)(&sym_handle2) = dlsym(handle2, "PluginHandle2");
    sym_handle(); 
    sym_handle2(); 

    dlclose(handle); 
    dlclose(handle2); 

    //ex1 - test !
    // void* handle = dlopen("./plugin_dll3.so",RTLD_NOW);
    // std::cout << "plugin_dll3.so : "; 
    // *(void**)(&sym_handle) = dlsym(handle, "PluginHandle");
    // sym_handle(); 

    return 0; 
}

