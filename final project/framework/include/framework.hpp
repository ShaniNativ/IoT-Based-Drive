/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/
#ifndef ILRD_RD1645_FRAMEWORK_HPP
#define ILRD_RD1645_FRAMEWORK_HPP

#include <list>
#include <string>
#include <functional>
#include <memory>

#include "reactor.hpp"
#include "dir_monitor.hpp"
#include "dll_loader.hpp"
#include "logger.hpp"

#include "input_proxy.hpp"
#include "icommand.hpp"

namespace ilrd
{


class Framework
{

public: 
    struct FDModeCallback;
    Framework(const std::list<FDModeCallback>& fd_list, 
        const std::string& path_name, const std::list< std::pair<int, std::function<std::shared_ptr<ICommand>()>>>& list_creators);
    void Run(); //blocking
    struct FDModeCallback
    {
        int fd; 
        ilrd::enums::mode mode;
        IInputProxy* input_proxy; 
    };

private: 
    const std::list<FDModeCallback> m_fd_list;
    const std::string& m_path_name; 
    const std::list< std::pair<int, std::function<std::shared_ptr<ICommand>()>>> m_list_creators;
    ilrd::LinuxListener m_listener;
    ilrd::Reactor m_reactor; 
    ilrd::DLLloader m_dll_loader; 
    ilrd::Callback<ilrd::DLLloader, const std::string&> m_callback_load;
    ilrd::DirMonitor m_monitor; 

};

} //ilrd


#endif //ILRD_RD1645_FRAMEWORK_HPP