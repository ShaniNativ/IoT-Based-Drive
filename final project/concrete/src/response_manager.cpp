/***
Written by: Shani Davidian
Reviewed by:
Date: 6.4.25
Exercise: Final Project - Master part
***/

#include <iostream> /* test only */

#include "response_manager.hpp"
#include "minion_manager.hpp"
#include "response.hpp"
#include "factory.hpp"
#include "ticket.hpp"

inline std::shared_ptr<ilrd::IResponse> CreateReadResponse()
{
    return std::make_shared<ilrd::ReadResponse>(ilrd::ReadResponse());
}

inline std::shared_ptr<ilrd::IResponse> CreateWriteResponse()
{
    return std::make_shared<ilrd::WriteResponse>(ilrd::WriteResponse());
}

ilrd::ResponseManager::~ResponseManager()
{
    delete m_callback_ticket_done;
    delete m_callback_new_ticket;
    delete m_callback_new_task;
}

void ilrd::ResponseManager::Init(ilrd::NBDProxy* nbd_proxy)
{

    m_nbd_proxy = nbd_proxy;
    m_callback_ticket_done = new ilrd::Callback<ilrd::ResponseManager,
                                                std::shared_ptr<ilrd::TicketResult>>(ilrd::Callback<ilrd::ResponseManager,
                                                                                                    std::shared_ptr<ilrd::TicketResult>>(*ilrd::Handleton<ResponseManager>::GetInstace(),
                                                                                                                                         &ilrd::ResponseManager::OnTicketDone));

    m_callback_new_ticket = new ilrd::Callback<ilrd::ResponseManager,
                                               std::shared_ptr<ilrd::Ticket>>(ilrd::Callback<ilrd::ResponseManager,
                                                                                             std::shared_ptr<ilrd::Ticket>>(*ilrd::Handleton<ResponseManager>::GetInstace(),
                                                                                                                            &ilrd::ResponseManager::OnTicketCreate));

    m_callback_new_task = new ilrd::Callback<ilrd::ResponseManager,
                                             std::shared_ptr<ilrd::ATaskArgs>>(ilrd::Callback<ilrd::ResponseManager,
                                                                                              std::shared_ptr<ilrd::ATaskArgs>>(*ilrd::Handleton<ResponseManager>::GetInstace(),
                                                                                                                                &ilrd::ResponseManager::OnNewTask));

    nbd_proxy->RegisterForNewTaskArgs(m_callback_new_task);

    ilrd::Handleton<ilrd::MinionManager>::GetInstace()->RegisterForNewTickets(m_callback_new_ticket);

    ilrd::Handleton<ilrd::Factory<ilrd::enums::mode, ilrd::IResponse>>::
        GetInstace()
            ->Register(ilrd::enums::mode::READ, CreateReadResponse);
    ilrd::Handleton<ilrd::Factory<ilrd::enums::mode, ilrd::IResponse>>::
        GetInstace()
            ->Register(ilrd::enums::mode::WRITE, CreateWriteResponse);
}

void ilrd::ResponseManager::OnTicketCreate(std::shared_ptr<ilrd::Ticket> ticket)
{
    m_map_uid_ticket[ticket->GetTicketUid()] = ticket;
    ticket->RegisterToResultDispatcher(m_callback_ticket_done);
}

void ilrd::ResponseManager::OnTicketDone(std::shared_ptr<ilrd::TicketResult> ticket) // ned to be task result and nor ticket ???
{
    m_map_uid_ticket.erase(ticket->GetTickeResultUid());

    if (ticket->GetStatus() == ilrd::enums::status::SUCCESS)
    {
        std::cout << "Response manager: Ticket Success" << std::endl;
        //m_nbd_proxy->NBDReadResponse(ticket->GetTickeResultUid(), ticket->GetStatus(), ticket->GetBuffer(), ticket->GetLength());
        m_map_uid_response[ticket->GetTickeResultUid()]->OnSuccess(m_nbd_proxy, ticket->GetTickeResultUid(), ticket->GetBuffer(), ticket->GetLength());

    }
    else
    {
        std::cout << "Response manager: Ticket Failure" << std::endl;
        m_map_uid_response[ticket->GetTickeResultUid()]->OnFailure(m_nbd_proxy, ticket->GetTickeResultUid());

        //m_nbd_proxy->NBDWriteResponse(ticket->GetTickeResultUid(), ticket->GetStatus());
    }

    m_map_uid_response.erase(ticket->GetTickeResultUid());
}

void ilrd::ResponseManager::OnNewTask(std::shared_ptr<ilrd::ATaskArgs> task_args)
{
    m_map_uid_response.emplace(task_args->GetUID(), ilrd::Handleton<ilrd::Factory<ilrd::enums::mode, ilrd::IResponse>>::
                                                        GetInstace()
                                                            ->Creator(static_cast<ilrd::enums::mode>(task_args->GetKey())));
}

bool ilrd::ResponseManager::IsTicketExist(Uid uid)
{
    return (m_map_uid_ticket.contains(uid)) && (m_map_uid_response.contains(uid));
}

void ilrd::ResponseManager::UnregisterFromTicket(ilrd::Uid uid)
{
    // may have race condition?????
    m_map_uid_ticket[uid]->UnregisterFromTicket(m_callback_ticket_done);
    OnTicketDone(std::make_shared<ilrd::TicketResult>(ilrd::TicketResult(uid, ilrd::enums::FAILURE, 0, nullptr)));
}
