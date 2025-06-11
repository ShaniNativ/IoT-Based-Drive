/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_RESPONSE_MANAGER_HPP
#define ILRD_RD1645_RESPONSE_MANAGER_HPP

#include <memory> /* std::shared_ptr */
#include <map> 

#include "nbd_proxy.hpp"
#include "handleton.hpp"
#include "dispatcher.hpp"
#include "ticket.hpp"
#include "response.hpp"

namespace ilrd
{
class ResponseManager
{

public:

    friend class ilrd::Handleton<ResponseManager>; 
    ~ResponseManager(); 
    void Init(ilrd::NBDProxy* nbd_proxy); 
    void OnTicketCreate(std::shared_ptr<ilrd::Ticket> ticket); 
    void OnTicketDone(std::shared_ptr<ilrd::TicketResult> ticket); 
    void OnNewTask(std::shared_ptr<ilrd::ATaskArgs> task_args);
    bool IsTicketExist(Uid uid);
    void UnregisterFromTicket(Uid uid);

private:

    ResponseManager() = default; //handleton
    ilrd::NBDProxy* m_nbd_proxy;
    std::map<ilrd::Uid, std::shared_ptr<ilrd::Ticket>> m_map_uid_ticket; 
    std::map<ilrd::Uid, std::shared_ptr<ilrd::IResponse>> m_map_uid_response; 
    ilrd::Callback<ilrd::ResponseManager,std::shared_ptr<ilrd::TicketResult>>* m_callback_ticket_done;
    ilrd::Callback<ilrd::ResponseManager,std::shared_ptr<ilrd::Ticket>>* m_callback_new_ticket; 
    ilrd::Callback<ilrd::ResponseManager,std::shared_ptr<ilrd::ATaskArgs>>* m_callback_new_task; 
}; 
}

#endif //ILRD_RD1645_RESPONSE_MANAGER_HPP
