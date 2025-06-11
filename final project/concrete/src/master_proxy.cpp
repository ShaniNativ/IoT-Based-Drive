/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/


#include "master_proxy.hpp"
#include "message.hpp"
#include "handleton.hpp"
#include "factory.hpp"

/*==================================== Static Functions ======================*/
std::shared_ptr<ilrd::AMessageRequest> ilrd::MasterProxy::CreateWriteMessageRequest()
{
    return std::make_shared<ilrd::MessageWriteRequest>(ilrd::MessageWriteRequest());
}

std::shared_ptr<ilrd::AMessageRequest> ilrd::MasterProxy::CreateReadMessageRequest()
{
    return std::make_shared<ilrd::MessageReadRequest>(ilrd::MessageReadRequest());
}


std::shared_ptr<ilrd::ITaskArgs> ilrd::MasterProxy::CreateReadArgs(std::shared_ptr<ilrd::MessageReadRequest> msg)
{
    std::shared_ptr<ilrd::ReadArgs> args(new ilrd::ReadArgs(msg->GetOffset(), msg->GetLength()));
    args->SetUID(msg->GetUid());

    return args; 
}

std::shared_ptr<ilrd::ITaskArgs> ilrd::MasterProxy::CreateWriteArgs(std::shared_ptr<ilrd::MessageWriteRequest> msg)
{
     std::shared_ptr<ilrd::WriteArgs> args(new ilrd::WriteArgs(msg->GetOffset(), msg->GetLength()));
     args->SetBuffer(msg->GetData().get()); 
     args->SetUID(msg->GetUid());

     return args; 
}

/*=============================================================================== */
ilrd::MasterProxy::MasterProxy()// the server bind
{
    Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>::
                GetInstace()->Register(ilrd::enums::message_type::READ_REQUEST, CreateReadMessageRequest); 
    Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>::
                GetInstace()->Register(ilrd::enums::message_type::WRITE_REQUEST, CreateWriteMessageRequest); 

}

void ilrd::MasterProxy::Init(int port)
{
    m_master_proxy_socket.UdpSocketBind(port); 
}

std::shared_ptr<ilrd::ITaskArgs> ilrd::MasterProxy::GetTaskArgs(int fd, ilrd::enums::mode mode)
{
    (void)mode; 
    (void)fd; 
    std::unique_lock<std::mutex> lock(m_socket_lock);

    char buffer[BUFSIZ] = {0}; 
    char* runner = buffer; 
    m_master_proxy_socket.SocketRcv(buffer, BUFSIZ); 

    uint64_t size = *(uint64_t*)runner;
    runner += sizeof(size); 
    ilrd::enums::message_type class_type = *(ilrd::enums::message_type*)runner;
    runner += sizeof(class_type);

    std::shared_ptr<AMessageRequest> msg = static_pointer_cast<AMessageRequest>(Handleton<ilrd::Factory
        <ilrd::enums::message_type, ilrd::AMessage>>::GetInstace()->Creator(class_type)); 
    msg->FromBuffer(runner); 

    if(class_type == ilrd::enums::message_type::READ_REQUEST)
    {
        return CreateReadArgs(std::static_pointer_cast<MessageReadRequest>(msg));

    }
    else
    {
        return CreateWriteArgs(std::static_pointer_cast<MessageWriteRequest>(msg));
    }

}

int ilrd::MasterProxy::GetFD()
{
    return m_master_proxy_socket.GetSocketFD();
}

void ilrd::MasterProxy::SendReadResponse(ilrd::Uid uid, ilrd::enums::status result, uint64_t buffer_len,  std::shared_ptr<char[]> buffer)
{
    std::unique_lock<std::mutex> lock(m_socket_lock);

    std::shared_ptr<ilrd::MessageReadResponse> msg(new ilrd::MessageReadResponse(uid, result, buffer_len, buffer)); 
    char* udp_buffer = new char[msg->GetSize()]; 
    msg->ToBuffer(udp_buffer); 
    m_master_proxy_socket.SocketSend(udp_buffer, msg->GetSize()); 

    delete[] udp_buffer; 
}

void ilrd::MasterProxy::SendWriteResponse(ilrd::Uid uid, ilrd::enums::status result)
{
    std::unique_lock<std::mutex> lock(m_socket_lock);

    std::shared_ptr<ilrd::MessageWriteResponse> msg(new ilrd::MessageWriteResponse(uid, result)); 
    char* udp_buffer = new char[msg->GetSize()]; 
    msg->ToBuffer(udp_buffer); 
    m_master_proxy_socket.SocketSend(udp_buffer, msg->GetSize()); 

    delete[] udp_buffer; 
}

