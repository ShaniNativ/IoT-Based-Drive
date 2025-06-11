/***
Written by: Shani Davidian
Reviewed by:
Date: 6.4.25
Exercise: Final Project - Master part
***/

#include <iostream>
#include <list>
#include <cstring>
#include "framework.hpp"
#include "nbd_proxy.hpp"
#include "nbd.hpp"
#include "response_manager.hpp"
#include "minion_manager.hpp"
#include "command.hpp"
#include "definitions.hpp"
#include "message.hpp"

using namespace ilrd;

// gdpp23 -fPIC -shared -I./framework/include -I./concrete/include framework/test/test_plugin_framework.cpp -o libdll.so -Wl,-rpath,. -L. -lshared_dll

// gdpp23 -fPIC -shared -o libshared_dll.so -I./framework/include -I./concrete/include framework/src/thread_pool.cpp framework/src/scheduler.cpp framework/src/logger.cpp concrete/src/master_proxy.cpp concrete/src/file_manager.cpp concrete/src/udp.cpp concrete/src/socket.cpp concrete/src/message.cpp framework/src/handleton.cpp concrete/src/minion_manager.cpp concrete/src/response_manager.cpp concrete/src/response.cpp concrete/src/task_args.cpp  concrete/src/minion_proxy.cpp concrete/src/nbd_proxy.cpp  concrete/src/ticket.cpp  concrete/src/uid.cpp framework/src/listener.cpp framework/src/reactor.cpp

// gdpp23 concrete/test/test_concrete.cpp   concrete/src/nbd.cpp concrete/src/command.cpp framework/src/framework.cpp framework/src/fw_tptask.cpp framework/src/dir_monitor.cpp framework/src/dll_loader.cpp framework/src/async_injection.cpp -I./framework/include -I./concrete/include -Wl,-rpath,. -L. -lshared_dll -o master.out

// gdpp23 concrete/test/test_minion.cpp concrete/src/minion_command.cpp framework/src/framework.cpp framework/src/fw_tptask.cpp framework/src/dir_monitor.cpp framework/src/dll_loader.cpp framework/src/async_injection.cpp -I./framework/include -I./concrete/include -Wl,-rpath,. -L. -lshared_dll -o minion.out

// aarch64-linux-gnu-g++ -std=c++23 -pedantic-errors -Wall -Wextra -g  concrete/test/test_concrete.cpp   concrete/src/nbd.cpp concrete/src/command.cpp framework/src/framework.cpp framework/src/fw_tptask.cpp framework/src/dir_monitor.cpp framework/src/dll_loader.cpp framework/src/async_injection.cpp -I./framework/include -I./concrete/include -Wl,-rpath,. -L. -lshared_dll -o master.out

// aarch64-linux-gnu-g++ -std=c++23 -pedantic-errors -Wall -Wextra -g concrete/test/test_minion.cpp concrete/src/minion_command.cpp framework/src/framework.cpp framework/src/fw_tptask.cpp framework/src/dir_monitor.cpp framework/src/dll_loader.cpp framework/src/async_injection.cpp -I./framework/include -I./concrete/include -Wl,-rpath,. -L. -lshared_dll -o minion.out

std::shared_ptr<ICommand> CreateReadCommand()
{
    return std::make_shared<CommandRead>(CommandRead());
};

std::shared_ptr<ICommand> CreateWriteCommand()
{
    return std::make_shared<CommandWrite>(CommandWrite());
}

int main(int argc, char* argv[])
{
    NBDProxy nbd_proxy;
    if(argc != 2)
    {
        return 0; 
    }
    NBD nbd(argv[1]);
    nbd.Init();
    
    Framework::FDModeCallback input_proxies = {nbd.GetNBD_FD(), ilrd::enums::mode::READ, &nbd_proxy};
    //Framework::FDModeCallback input_proxies = {STDIN_FILENO, ilrd::enums::mode::WRITE, &nbd_proxy};
    std::list<Framework::FDModeCallback> input_proxies_list = {input_proxies};
    std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>> creators =
        {{ilrd::enums::mode::READ, CreateReadCommand}, {ilrd::enums::mode::WRITE, CreateWriteCommand}};

    Handleton<ilrd::MinionManager>::GetInstace()->Init();
    Handleton<ilrd::ResponseManager>::GetInstace()->Init(&nbd_proxy);

    Framework fw(input_proxies_list, "./plugins", creators);

    fw.Run();

    return 0;
}
