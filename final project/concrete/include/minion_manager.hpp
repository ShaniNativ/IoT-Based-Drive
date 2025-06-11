/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_MINION_MANAGER_HPP
#define ILRD_RD1645_MINION_MANAGER_HPP

#include <string>

#include "thread"
#include "ticket.hpp"
#include "dispatcher.hpp"
#include "handleton.hpp"
#include "minion_proxy.hpp"
#include "task_args.hpp"
#include "reactor.hpp"

namespace ilrd
{

class MinionManager 
{
public:
    
    void AddReadTask(uint64_t offset, uint32_t length, ilrd::Uid uid);
    void AddWriteTask(uint64_t offset, uint32_t length, ilrd::Uid uid, std::shared_ptr<char[]> data);
    void RegisterForNewTickets(Acallback<std::shared_ptr<Ticket>>* obj); 
    void Init(); 
    void ThreadHandle();
    

private:
    MinionManager() = default; //private ctor = handleton class
    std::vector<std::shared_ptr<ilrd::IMinionProxy>> m_minions; 
    ilrd::Dispatcher<std::shared_ptr<Ticket>> m_dispatcher_new_ticket; 
    static ilrd::LinuxListener m_listener;
    static ilrd::Reactor m_reactor;
    static const int m_no_minions; 
    static const int m_minion_block_size; 
    static const int m_total_block_size; 
    static const std::string ips[3]; 
    std::jthread m_reactor_thread; 
    friend class ilrd::Handleton<MinionManager>;
};


}

#endif //ILRD_RD1645_MINION_MANAGER_HPP
