/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Minion part
***/
#include <iostream>


#include "minion_command.hpp"
#include "task_args.hpp"
#include "file_manager.hpp"
#include "master_proxy.hpp"

std::pair<std::function<bool()>, std::chrono::milliseconds> ilrd::MinionCommandRead::Run(std::shared_ptr<ITaskArgs> args)
{
    std::shared_ptr<ilrd::ReadArgs> read_args = static_pointer_cast<ilrd::ReadArgs>(args); 
    std::shared_ptr<char[]> buffer(new char[read_args->GetLength()]); 
    ilrd::enums::status result = ilrd::Handleton<ilrd::FileManager>::GetInstace()->Read(read_args->GetLength(),  read_args->GetOffset(), buffer); 

    ilrd::Handleton<ilrd::MasterProxy>::GetInstace()->SendReadResponse(read_args->GetUID(), result, read_args->GetLength(), buffer); 
    
    return std::make_pair(nullptr, std::chrono::milliseconds(0));
}


std::pair<std::function<bool()>, std::chrono::milliseconds> ilrd::MinionCommandWrite::Run(std::shared_ptr<ITaskArgs> args)
{
    std::shared_ptr<ilrd::WriteArgs> write_args = static_pointer_cast<ilrd::WriteArgs>(args); 
    ilrd::enums::status result = ilrd::Handleton<ilrd::FileManager>::GetInstace()->Write( write_args->GetLength(),  write_args->GetOffset(), write_args->GetBuffer()); 

    ilrd::Handleton<ilrd::MasterProxy>::GetInstace()->SendWriteResponse( write_args->GetUID(), result); 

    return std::make_pair(nullptr, std::chrono::milliseconds(0));
}
