/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#include "fw_tptask.hpp"

#include "icommand.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "logger.hpp"
#include "async_injection.hpp"

/*============================  FW_TPTask Implementationn =====================*/

ilrd::thread_pool::FW_TPTask::FW_TPTask(std::shared_ptr<ITaskArgs> args) : m_p_tasks_args(args)
{
}

void ilrd::thread_pool::FW_TPTask::Run()
{
    try
    {
        std::shared_ptr<ilrd::ICommand> p_command(ilrd::Handleton<ilrd::Factory<int,
                                                                                ilrd::ICommand>>::GetInstace()
                                                      ->Creator(m_p_tasks_args->GetKey())); // try catch to write to logger
        std::pair<std::function<bool()>, std::chrono::milliseconds>
            async_injection_and_interval = p_command->Run(m_p_tasks_args);

        if (async_injection_and_interval.first)
        {
            new ilrd::AsyncInjection(async_injection_and_interval.first,
                                     async_injection_and_interval.second);
        }
    }
    catch (std::exception& e)
    {
        ilrd::Handleton<ilrd::Logger>::GetInstace()->Log
            (std::string("FW_TPTask Failed to create Icommand ") + e.what(), ilrd::Logger::ERROR); 
    }
}