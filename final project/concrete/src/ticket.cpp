/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <iostream> /* for test */
#include "ticket.hpp"

/*========================================= TicketResult ======================*/

ilrd::TicketResult::TicketResult(ilrd::Uid uid, ilrd::enums::status status, 
    uint64_t len, std::shared_ptr<char[]> buf) : m_uid(uid), m_status(status), m_len(len) , m_buf(buf)

{

}

ilrd::enums::status ilrd::TicketResult::GetStatus()
{
    return m_status; 
}

ilrd::Uid ilrd::TicketResult::GetTickeResultUid()
{
    return m_uid;
}

uint64_t ilrd::TicketResult::GetLength()
{
    return m_len;
}

std::shared_ptr<char[]> ilrd::TicketResult::GetBuffer()
{
    return m_buf; 
}

/*========================================= Ticket ============================*/

ilrd::Ticket::Ticket(ilrd::Uid uid, std::list<std::shared_ptr<ilrd::IMinionProxy>> minions) : 

m_uid(uid), m_minion_proxy_list(minions), m_minions_count(m_minion_proxy_list.size())

{
}

void ilrd::Ticket::RegisterToResultDispatcher(ilrd::Acallback<std::shared_ptr<ilrd::TicketResult>>* obj)
{
    m_result_dispatcher.Register(obj); 
}

void ilrd::Ticket::UnregisterFromTicket(ilrd::Acallback<std::shared_ptr<ilrd::TicketResult>>* obj)
{
    m_result_dispatcher.Unregister(obj); 
}

ilrd::Uid ilrd::Ticket::GetTicketUid()
{
    return m_uid;
}

void ilrd::Ticket::OnMinionProxyDone(ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf)
{
    if(--m_minions_count == 0)
    {
        m_result_dispatcher.Notify(std::make_shared<ilrd::TicketResult>(ilrd::TicketResult(GetTicketUid(), status, len, buf)));
    }
}


