/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_MINION_PROXY_HPP
#define ILRD_RD1645_MINION_PROXY_HPP

#include <memory>
#include <queue>
#include <functional>
#include <map>
#include <mutex>

#include "dispatcher.hpp"
#include "message.hpp"
#include "definitions.hpp"
#include "udp.hpp"

namespace ilrd
{
class IMinionProxy
{
public:
    virtual void AddWriteTask(size_t offset,  size_t length, std::shared_ptr<char[]> data, Uid uid,
        std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done)  = 0; 
    virtual void AddReadTask(size_t offset,  size_t length, Uid uid, 
        std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done)  = 0;
    virtual int GetMinionFD() = 0; 
    virtual void OnMinionFDWakeup(int fd, ilrd::enums::mode mode) = 0; 
}; 

class MinionProxy : public IMinionProxy
{

public:
    MinionProxy(int port, std::shared_ptr<const char> ip); 
    void AddWriteTask(size_t offset, size_t length, std::shared_ptr<char[]> data, Uid uid,
        std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done); // ?pointer? acallback to function??
    void AddReadTask(size_t offset, size_t length, Uid uid, 
        std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)> on_done);
    int GetMinionFD(); 
    void OnMinionFDWakeup(int fd, ilrd::enums::mode mode);//??? 
    void SendMsg(std::shared_ptr<AMessageRequest> msg_request);
private:

    static std::shared_ptr<ilrd::AMessageResponse> CreateWriteMessageResponse(); 
    static std::shared_ptr<ilrd::AMessageResponse> CreateReadMessageResponse(); 
    std::map<ilrd::Uid, std::function<void(ilrd::enums::status, uint64_t, std::shared_ptr<char[]>)>> m_map_uid_ondone_fun; 
    int m_port;
    std::shared_ptr<const char> m_minion_ip; 
    Udp m_minion_proxy_socket; 
    std::mutex m_socket_mutex;

}; 

}

#endif //ILRD_RD1645_IMINION_PROXY_HPP