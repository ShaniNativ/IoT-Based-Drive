/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 23.3.25
Exercise: Reactor
***/

#include <reactor.hpp>

/*============================ IMPLEMENTATION ================================*/

ilrd::Reactor::Reactor(ilrd::IListener& listener) : m_listener(listener), 
                                                        m_is_running(true)
{

}

void ilrd::Reactor::Register(int fd, ilrd::enums::mode mode, 
                        std::function<void (int, ilrd::enums::mode)> callback)
{
    m_fd_map.insert({{fd, mode}, callback}); /* ? syntax */
}

void ilrd::Reactor::Unregister(int fd, ilrd::enums::mode mode)
{
    m_fd_map.erase({fd, mode});
}

void ilrd::Reactor::Run()
{
    while(m_is_running)
    {
        std::vector<std::pair<int, ilrd::enums::mode>> fd_mode_vec; 

        for(auto it = m_fd_map.begin(); it != m_fd_map.end(); ++it)
        {
            fd_mode_vec.emplace_back(it->first); 
        }

        std::vector<std::pair<int, ilrd::enums::mode>> active_fd_vec = m_listener.Listen(fd_mode_vec); 
       for(auto it = active_fd_vec.begin(); it != active_fd_vec.end(); ++it)
        {
            m_fd_map[*it](it->first, it->second); 
        }
    }
}

void ilrd::Reactor::Stop()
{
    m_is_running = false; 
}


