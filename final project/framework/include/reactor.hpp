/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 23.3.25
Exercise: Reactor
***/

#ifndef ILRD_RD1645_REACTOR_HPP
#define ILRD_RD1645_REACTOR_HPP

#include <functional>
#include <map>

#include "listener.hpp"
#include "definitions.hpp"

namespace ilrd
{

class Reactor
{
public:
    Reactor(IListener& listener); 
    void Register(int fd, ilrd::enums::mode mode, std::function<void(int, ilrd::enums::mode)> callback); 
    void Unregister(int fd, ilrd::enums::mode mode); 
    void Run(); //blocking
    void Stop(); 

private:
    std::map<std::pair<int, ilrd::enums::mode>, std::function<void(int, ilrd::enums::mode)>> m_fd_map;
    IListener& m_listener; 
    bool m_is_running; 
};

}//ilrd

#endif //ILRD_RD1645_REACTOR_HPP