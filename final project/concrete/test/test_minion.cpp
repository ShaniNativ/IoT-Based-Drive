/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/
#include <cstring>
#include "minion_command.hpp"
#include "master_proxy.hpp"
#include "handleton.hpp"
#include "framework.hpp"
#include "file_manager.hpp"

using namespace ilrd;

std::shared_ptr<ICommand> CreateMinionReadCommand()
{
    return std::make_shared<MinionCommandRead>(MinionCommandRead());
};

std::shared_ptr<ICommand> CreateMinionWriteCommand()
{
    return std::make_shared<MinionCommandWrite>(MinionCommandWrite());
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Run with socket port and with drive.dat name" << std::endl; 
        return 0;
    }

    int port = atoi(argv[1]);

    
    Framework::FDModeCallback input_proxies = {Handleton<MasterProxy>::GetInstace()->GetFD(), ilrd::enums::mode::READ, Handleton<MasterProxy>::GetInstace()};
    std::list<Framework::FDModeCallback> input_proxies_list = {input_proxies};
    std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>> creators =
        {{ilrd::enums::mode::READ, CreateMinionReadCommand}, {ilrd::enums::mode::WRITE, CreateMinionWriteCommand}};
    
    Framework fw(input_proxies_list, "./plugins", creators);

    Handleton<FileManager>::GetInstace()->Init(argv[2]); 

    Handleton<MasterProxy>::GetInstace()->Init(port);  //to bind the port
    fw.Run();

    return 0;
}
