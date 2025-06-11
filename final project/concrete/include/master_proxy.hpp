/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/

#ifndef ILRD_RD1645_MASTER_PROXY_HPP
#define ILRD_RD1645_MASTER_PROXY_HPP

#include <memory>
#include <mutex>

#include "input_proxy.hpp"
#include "task_args.hpp"
#include "definitions.hpp"
#include "udp.hpp"
#include "message.hpp"
#include "handleton.hpp"

namespace ilrd
{
class MasterProxy : public IInputProxy
{
public:
    void Init(int port); 
    std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, ilrd::enums::mode mode); 
    int GetFD(); 
    void SendReadResponse(ilrd::Uid uid, ilrd::enums::status result, uint64_t buffer_len, std::shared_ptr<char[]> buffer); 
    void SendWriteResponse(ilrd::Uid uid, ilrd::enums::status result);
    friend class Handleton<MasterProxy>; 

private:
    MasterProxy();
    std::shared_ptr<ITaskArgs> CreateReadArgs(std::shared_ptr<MessageReadRequest> msg);
    std::shared_ptr<ITaskArgs> CreateWriteArgs(std::shared_ptr<MessageWriteRequest> msg);
    static std::shared_ptr<ilrd::AMessageRequest> CreateWriteMessageRequest(); 
    static std::shared_ptr<ilrd::AMessageRequest> CreateReadMessageRequest();
    Udp m_master_proxy_socket; 
    std::mutex m_socket_lock; 
};
}

#endif //ILRD_RD1645_MASTER_PROXY_HPP