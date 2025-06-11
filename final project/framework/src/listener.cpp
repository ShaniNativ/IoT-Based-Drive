/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 23.3.25
Exercise: Reactor
***/
#include <vector> /* std::vector */
#include "listener.hpp"

/*====================== IMPLEMENTATION Listener ==============================*/

int ilrd::LinuxListener::FDInit(std::vector<std::pair<int, ilrd::enums::mode>> vector)
{
    int max_fd = 0; 
    
    FD_ZERO(&m_fd_readset); 
    FD_ZERO(&m_fd_writeset); 

    for(auto iter = vector.begin(); iter != vector.end(); ++ iter)
    {
        if(iter->second == ilrd::enums::READ)
        {
            FD_SET(iter->first, &m_fd_readset);
        }
        else
        {
            FD_SET(iter->first, &m_fd_writeset); 
        }

        if(iter->first > max_fd)
        {
            max_fd = iter->first;
        }         
    }

    return max_fd;
}

std::vector<std::pair<int, ilrd::enums::mode>> ilrd::LinuxListener::Listen
                        (std::vector<std::pair<int, ilrd::enums::mode>> vector)
{
    std::vector<std::pair<int, ilrd::enums::mode>> active_fd_vec; 
    int max_fd = FDInit(vector); 
    int fd_ready = select(max_fd + 1, &m_fd_readset, &m_fd_writeset, NULL, NULL); 

    if(fd_ready <= 0)
    {
        //throw
    }
    else
    {
        auto iter = vector.begin(); 
        while(iter != vector.end())
        {
            if(FD_ISSET(iter->first, &m_fd_readset) || FD_ISSET(iter->first, &m_fd_writeset))
            {
                active_fd_vec.emplace_back(*iter); 
            }

            ++iter; 
        }
    }

    return active_fd_vec; 
}




