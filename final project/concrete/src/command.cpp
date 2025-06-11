
/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#include <iostream>

#include "command.hpp"
#include "task_args.hpp"
#include "handleton.hpp"
#include "minion_manager.hpp"
#include "response_manager.hpp"

ilrd::ErrorHandling::ErrorHandling(Uid uid) : m_async_injection_calls(0), m_uid(uid)
{

}

bool ilrd::ErrorHandling::operator()()
{
    if(ilrd::Handleton<ResponseManager>::GetInstace()->IsTicketExist(m_uid) && m_async_injection_calls < 10)
    {
        ++m_async_injection_calls; 
        return true; 
    }
    else if(ilrd::Handleton<ResponseManager>::GetInstace()->IsTicketExist(m_uid) && m_async_injection_calls >= 10)
    {
        ilrd::Handleton<ResponseManager>::GetInstace()->UnregisterFromTicket(m_uid); 
    }

    return false;
}

ilrd::CommandRead::CommandRead()
{
}

std::pair<std::function<bool()>, std::chrono::milliseconds> ilrd::CommandRead::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    std::shared_ptr<ilrd::ReadArgs> read_args = static_pointer_cast<ilrd::ReadArgs>(args); 
    ilrd::Handleton<MinionManager>::GetInstace()->AddReadTask(read_args->GetOffset(), read_args->GetLength(), read_args->GetUID());
    Uid uid = read_args->GetUID();

    //return std::make_pair(nullptr, std::chrono::milliseconds(0)); 
    std::shared_ptr<ilrd::ErrorHandling> my_functor(new ilrd::ErrorHandling(uid));
    return std::make_pair(*my_functor.get(), std::chrono::milliseconds(100)); 
}

ilrd::CommandWrite::CommandWrite()
{
}

std::pair<std::function<bool ()>, std::chrono::milliseconds> ilrd::CommandWrite::Run(std::shared_ptr<ilrd::ITaskArgs> args)
{
    std::shared_ptr<ilrd::WriteArgs> write_args = static_pointer_cast<ilrd::WriteArgs>(args); 
    ilrd::Handleton<MinionManager>::GetInstace()->AddWriteTask(write_args->GetOffset(), write_args->GetLength(), write_args->GetUID(), write_args->GetBuffer());
    Uid uid = write_args->GetUID();
    std::shared_ptr<ilrd::ErrorHandling> my_functor(new ilrd::ErrorHandling(uid));
    
    return std::make_pair(*my_functor.get(), std::chrono::milliseconds(100)); 
}

