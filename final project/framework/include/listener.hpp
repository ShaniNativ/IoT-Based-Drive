
/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 23.3.25
Exercise: Reactor
***/

#ifndef ILRD_RD1645_LISTENER_HPP
#define ILRD_RD1645_LISTENER_HPP

#include <vector>
#include "definitions.hpp"
namespace ilrd
{
    class IListener
    {
    public:
        enum mode
        {
            READ, 
            WRITE, 
        };
        virtual std::vector<std::pair<int, ilrd::enums::mode>> 
                    Listen(std::vector<std::pair<int, ilrd::enums::mode>> vector) = 0; 
    };
    
    class LinuxListener : public IListener
    {
    public:
        LinuxListener() = default; 
        std::vector<std::pair<int, ilrd::enums::mode>> 
                    Listen(std::vector<std::pair<int, ilrd::enums::mode>> vector); 
    private:
        int FDInit(std::vector<std::pair<int, ilrd::enums::mode>> vector);
        
        fd_set m_fd_readset;
        fd_set m_fd_writeset;
    };
}

#endif //ILRD_RD1645_LISTENER_HPP


