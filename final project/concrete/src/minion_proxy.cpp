/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <iostream>
#include "minion_proxy.hpp"
#include "message.hpp"
#include "handleton.hpp"
#include "factory.hpp"

std::shared_ptr<ilrd::AMessageResponse> ilrd::MinionProxy::CreateWriteMessageResponse()
{
    return std::make_shared<ilrd::MessageWriteResponse>(ilrd::MessageWriteResponse());
}

std::shared_ptr<ilrd::AMessageResponse> ilrd::MinionProxy::CreateReadMessageResponse()
{
    return std::make_shared<ilrd::MessageReadResponse>(ilrd::MessageReadResponse());
}


/*===========================  Minion Proxy ===================================*/

ilrd::MinionProxy::MinionProxy(int port, std::shared_ptr<const char> ip) :m_port(port), m_minion_ip(ip), m_minion_proxy_socket()
{
    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>::
                GetInstace()->Register(ilrd::enums::message_type::READ_RESPONSE, ilrd::MinionProxy::CreateReadMessageResponse); 
    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>::
                GetInstace()->Register(ilrd::enums::message_type::WRITE_RESPONSE, ilrd::MinionProxy::CreateWriteMessageResponse); 
}

void ilrd::MinionProxy::AddWriteTask(size_t offset, size_t length, std::shared_ptr<char[]> data, Uid uid, 
                                     std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done)
{
    std::unique_lock<std::mutex> lock(m_socket_mutex);
    m_map_uid_ondone_fun[uid] = on_done; 
    std::shared_ptr<ilrd::MessageWriteRequest> msg(new ilrd::MessageWriteRequest(uid, length, offset, data)); 
    SendMsg(msg) ; 
}

void ilrd::MinionProxy::AddReadTask(size_t offset, size_t length, ilrd::Uid uid, 
    std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done)
{
    std::unique_lock<std::mutex> lock(m_socket_mutex);
    m_map_uid_ondone_fun[uid] = on_done; 
    std::shared_ptr<ilrd::MessageReadRequest> msg (new ilrd::MessageReadRequest(uid, length, offset)); 
    SendMsg(msg) ;   
}

int ilrd::MinionProxy::GetMinionFD()
{
    return m_minion_proxy_socket.GetSocketFD(); //for test only! it should be m_fd
}

void ilrd::MinionProxy::OnMinionFDWakeup(int fd, ilrd::enums::mode mode)
{
    (void)mode; 
    (void)fd; 
    std::unique_lock<std::mutex> lock(m_socket_mutex);
    char buffer[BUFSIZ] = {0}; 
    char* runner = buffer; 
    m_minion_proxy_socket.SocketRcv(buffer, BUFSIZ); 

    uint64_t size = *(uint64_t*)runner;
    runner += sizeof(size); 
    ilrd::enums::message_type class_type = *(ilrd::enums::message_type*)runner;
    runner += sizeof(class_type);

    std::shared_ptr<AMessageResponse> msg = static_pointer_cast<AMessageResponse>(Handleton<ilrd::Factory
        <ilrd::enums::message_type, ilrd::AMessage>>::GetInstace()->Creator(class_type)); 
    msg->FromBuffer(runner); 
  //  msg->GetUid().PrintUID(); 
    m_map_uid_ondone_fun.at(msg->GetUid())(msg->GetStatus(), msg->GetLength(), msg->GetBuffer()); // send this to the ticket with on_done - need to add two params - status and buffer.
}

void ilrd::MinionProxy::SendMsg(std::shared_ptr<AMessageRequest> msg_request)
{
    char* buffer = new char[msg_request->GetSize()]; 
    msg_request->ToBuffer(buffer); 
    m_minion_proxy_socket.ClientSend(buffer, msg_request->GetSize(), m_minion_ip.get(), m_port);
    delete[] buffer; 
}
