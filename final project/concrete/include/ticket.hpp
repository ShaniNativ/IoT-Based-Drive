/***
Written by: Shani Davidian
Reviewed by:
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_TICKET_HPP
#define ILRD_RD1645_TICKET_HPP

#include <mutex>

#include "uid.hpp"
#include "dispatcher.hpp"
#include "minion_proxy.hpp"
#include "definitions.hpp"
namespace ilrd
{

class TicketResult //??????
{
public:
    TicketResult(ilrd::Uid uid, ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf) ;
    ilrd::enums::status GetStatus(); 
    ilrd::Uid GetTickeResultUid(); 
    uint64_t GetLength(); 
    std::shared_ptr<char[]> GetBuffer(); 
    
private:
    ilrd::Uid m_uid;
    ilrd::enums::status m_status; 
    uint64_t m_len; 
    std::shared_ptr<char[]> m_buf;
    
};

class Ticket
{
    
public:
    Ticket(ilrd::Uid uid, std::list< std::shared_ptr<ilrd::IMinionProxy>> minions);
    void RegisterToResultDispatcher(ilrd::Acallback< std::shared_ptr<ilrd::TicketResult>>* obj);
    void UnregisterFromTicket(ilrd::Acallback< std::shared_ptr<ilrd::TicketResult>>* obj);
    ilrd::Uid GetTicketUid(); 
    //ilrd::enums::status GetStatus(); 
    void OnMinionProxyDone(ilrd::enums::status, uint64_t len, std::shared_ptr<char[]>); //??? what does ut gets?
  //  TicketResult& GetTaskResult(); 
    
    //function with response 

private:
    ilrd::Uid m_uid;
    ilrd::Dispatcher<std::shared_ptr<ilrd::TicketResult>> m_result_dispatcher; //?? 
    std::list< std::shared_ptr<ilrd::IMinionProxy>> m_minion_proxy_list;
    std::atomic<size_t> m_minions_count; 
};

}

#endif // ILRD_RD1645_TICKET_HPP
