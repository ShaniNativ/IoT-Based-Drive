/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Dir Monitor
***/

#include <filesystem>

#include "dir_monitor.hpp"

static const int MAX_EVENTS = 1024; 
static const int LEN_NAME = 15; 
static const int EVENT_SIZE  = (sizeof(struct inotify_event)); /*size of one event*/
static const int BUF_LEN = (MAX_EVENTS * (EVENT_SIZE + LEN_NAME)); /*buffer to store the data of events*/
/*============================= IMPLEMENTATION =============================*/

ilrd::DirMonitor::DirMonitor(const std::string& path_name) : 
                                        m_fd(inotify_init()),
                                        m_is_running(true), m_dir_name(path_name)
{
    if(-1 == m_fd)
    {
        throw std::runtime_error("Failed to initializes a new inotify instance");
    }

    m_wd = inotify_add_watch(m_fd, path_name.c_str(), IN_CLOSE_WRITE | IN_DELETE);

    if(-1 == m_wd)
    {
        throw std::runtime_error("Failed to initializes a new inotify instance");
    }
}

ilrd::DirMonitor::~DirMonitor()
{
    m_is_running = false;
    try
    {
        inotify_rm_watch(m_fd, m_wd);
        close(m_fd);
    }
    catch(...)
    {
        
    }
}

void ilrd::DirMonitor::RegisterLoad(ilrd::Acallback<const std::string&>* obj)
{
    m_dispatcher_load_event.Register(obj);
}

void ilrd::DirMonitor::RegisterUnLoad(ilrd::Acallback<const std::string&>* obj)
{
    m_dispatcher_unload_event.Register(obj);
}

void ilrd::DirMonitor::UnregisterLoad(ilrd::Acallback<const std::string&>* obj)
{
    m_dispatcher_load_event.Unregister(obj);
}

void ilrd::DirMonitor::UnregisterUnLoad(ilrd::Acallback<const std::string&>* obj)
{
    m_dispatcher_unload_event.Unregister(obj);
}

void ilrd::DirMonitor::Run()
{
    m_thread = std::jthread(&ilrd::DirMonitor::ThreadMonitor, this);
}

void ilrd::DirMonitor::ThreadMonitor()
{
    char buffer[BUF_LEN] = {0}; 

    while(m_is_running)
    {
        int length = read(m_fd, buffer, BUF_LEN);  

        if (length < 0) 
        {
           throw std::runtime_error("Faile to read"); 
        }  

        for (int i = 0; i < length;) 
        {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];

            if ((event->mask & IN_CLOSE_WRITE) && !(event->mask & IN_ISDIR))
            {
                m_dispatcher_load_event.Notify(m_dir_name + "/" + event->name);
            }
            
            else if ((event->mask & IN_DELETE) && !(event->mask & IN_ISDIR)) 
            {
                m_dispatcher_unload_event.Notify(m_dir_name + "/" + event->name);
            } 
            
            i += EVENT_SIZE + event->len;
        }
    }
}



