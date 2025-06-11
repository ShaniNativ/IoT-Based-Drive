/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#include "framework.hpp"

#include "itask_args.hpp"
#include "fw_tptask.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "async_injection.hpp"

/*============================  Framwork Implementationn =====================*/

ilrd::Framework::Framework(const std::list<FDModeCallback>& fd_list, 
    const std::string& path_name, const std::list< std::pair<int, std::function<std::shared_ptr<ICommand>()>>>& list_creators) :
    m_fd_list(fd_list), m_path_name(path_name), m_list_creators(list_creators), m_reactor(m_listener), 
    m_callback_load(m_dll_loader, &ilrd::DLLloader::Load), m_monitor(m_path_name)
{
    for(auto it = m_fd_list.begin(); it != m_fd_list.end(); ++it)
    {
        m_reactor.Register(it->fd, it->mode, [it](int fd, ilrd::enums::mode mode)
                        {    
                            std::shared_ptr<ilrd::ITaskArgs> p_task_args = it->input_proxy->GetTaskArgs(fd, mode); 

                            if(nullptr != p_task_args)
                            {
                                std::shared_ptr<ilrd::thread_pool::FW_TPTask> fw_task(new ilrd::thread_pool::FW_TPTask(p_task_args));
                                ilrd::Handleton<ilrd::ThreadPool>::GetInstace()->AddTask(fw_task);
                            }

                        } );

    }

    for(auto it = m_list_creators.begin(); it != m_list_creators.end(); ++it)
    {
        ilrd::Handleton<ilrd::Factory<int, ilrd::ICommand>>::GetInstace()->Register(it->first, it->second); 

    }

    m_monitor.RegisterLoad(&m_callback_load);
}

void ilrd::Framework::Run()
{
    m_monitor.Run(); 
    m_reactor.Run(); 
}

