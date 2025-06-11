/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Dir Monitor
***/

#ifndef ILRD_RD1645_DIRMONITOR_HPP
#define ILRD_RD1645_DIRMONITOR_HPP

#include <string> /* std::string */
#include <thread>
#include <sys/inotify.h>

#include "dispatcher.hpp" /* API */

namespace ilrd
{

class DirMonitor
{
public:
    DirMonitor(const std::string& path_name);
    ~DirMonitor();
    void RegisterLoad(ilrd::Acallback<const std::string&>* obj);  
    void RegisterUnLoad(ilrd::Acallback<const std::string&>* obj); 
    void UnregisterLoad(ilrd::Acallback<const std::string&>* obj); 
    void UnregisterUnLoad(ilrd::Acallback<const std::string&>* obj); 
    //Must register all callback befor run
    void Run(); //not blocking
private:
    ilrd::Dispatcher<const std::string&> m_dispatcher_load_event; 
    ilrd::Dispatcher<const std::string&> m_dispatcher_unload_event; 
    int m_fd;
    bool m_is_running;
    std::string m_dir_name; 
    int m_wd; 
    std::jthread m_thread; 
    void ThreadMonitor(); 
}; 


}//ilrd

#endif //ILRD_RD1645_DIRMONITOR_HPP
