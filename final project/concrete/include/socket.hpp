/*** 
Written by: Shani Davidian
Reviewed by: Ofir Nahshoni
Date: 23.2.25
Exercise: TCP\UDP Ping Pong
***/

#ifndef ILRD_RD1645_SOCKET_HPP
#define ILRD_RD1645_SOCKET_HPP

#if __cplusplus < 201103L
#  define noexcept throw()
#endif

#include <netinet/in.h> /*  struct sockaddr_in */
#include <exception>
#include <iostream>

namespace ilrd
{
class Socket
{
public:
    Socket(int socket_domain, int socket_type); 
    virtual ~Socket() = 0;
    int GetSocketFD() const noexcept;
    void SocketBind(int port);
    virtual int SocketRcv(char* buffer, size_t max_len) = 0;
    virtual void SocketSend(const char* msg, size_t msg_len) = 0;
    
protected:   
    int m_fd; 
    int m_socket_domain; 
    int m_socket_type;
    void CreateSockAdrr(sockaddr_in* destAddr, const char* buf, int port); 

private:
    Socket(const Socket& other);
    void operator=(const Socket& other); 
};
}

#endif
