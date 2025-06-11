/*** 
Written by: Shani Davidian
Reviewed by: Ofir Nahshoni
Date: 23.2.25
Exercise: TCP\UDP Ping Pong
***/

#include <sys/socket.h> /* socket */
#include <cstdio> /* puts */

#include "udp.hpp" /* API */

ilrd::Udp::Udp() : Socket(AF_INET, SOCK_DGRAM)
{

}


void ilrd::Udp::UdpSocketBind(int port)
{
    SocketBind(port); 
}

ilrd::Udp::~Udp() noexcept
{
}

void ilrd::Udp::ClientSend(const char* msg, size_t msg_len, const char *foreign_ip, int port)
{
    sockaddr_in dest_addr;

    CreateSockAdrr(&dest_addr, foreign_ip, port); 

    if(-1 == sendto(m_fd, msg, msg_len, 0, (struct sockaddr*)&dest_addr, sizeof(sockaddr_in)))
    {
        throw std::runtime_error("Failed to sendto");
    }
}

void ilrd::Udp::SocketSend(const char* msg, size_t msg_len)

{
    if(-1 == sendto(m_fd, msg, msg_len, 0, (struct sockaddr*)&m_connect_ip, sizeof(sockaddr_in)))
    {
        throw std::runtime_error("Failed to SocketSend: sendto");
    }
    
}

int ilrd::Udp::SocketRcv(char* buffer, size_t max_len)
{
    socklen_t add_len = sizeof(sockaddr_in);

    ssize_t bytes = recvfrom(m_fd, buffer, max_len - 1, 0, (struct sockaddr*)&m_connect_ip, 
    &add_len); 

    if(-1 == bytes)
    {
        throw std::runtime_error("Failed to SocketRcv: recvfrom");
    }

    buffer[bytes] = '\0'; 
    return bytes; 
}

void ilrd::Udp::SetBroadcast()
{
    int broadcast = 1; 

    if (setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, 
                                                    sizeof(broadcast)) == -1) 
    {
        throw std::runtime_error("Failed to SetBroadcast");
    }
}





