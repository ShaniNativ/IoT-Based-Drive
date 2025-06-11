/*** 
Written by: Shani Davidian
Reviewed by: Ofir Nahshoni
Date: 23.2.25
Exercise: TCP\UDP Ping Pong
***/


#ifndef ILRD_RD1645_UDP_HPP
#define ILRD_RD1645_UDP_HPP

#include "socket.hpp"

namespace ilrd
{
class Udp : public Socket
{
public:
    Udp(); 
    ~Udp() noexcept;
    void UdpSocketBind(int port); 
    void SocketSend(const char* msg, size_t msg_len); 
    void ClientSend(const char* msg, size_t msg_len, const char* foreign_ip, int port); 
    int SocketRcv(char* buffer, size_t max_len); 
    void SetBroadcast();
private:
    sockaddr_in m_connect_ip; 
};
}

#endif
