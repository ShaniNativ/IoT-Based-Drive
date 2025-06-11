#include <sys/socket.h> /* socket */
#include <unistd.h> /* close */
#include <cstring> /* memset */
#include <arpa/inet.h> /* inet_pton */
#include <cstdio> /* puts */

#include "socket.hpp"

ilrd::Socket::Socket(int socket_domain, int socket_type) :
                                m_fd(socket(socket_domain, socket_type, 0)), 
                                m_socket_domain(socket_domain), 
                                m_socket_type(socket_type) 
{
    if(-1 == m_fd)
    {
        throw std::runtime_error("Failed to create socket");
    }
}

ilrd::Socket::~Socket() 
{
    close(m_fd); 
}

int ilrd::Socket::GetSocketFD() const noexcept
{
    return m_fd; 
}

ilrd::Socket::Socket(const ilrd::Socket &other) 
{
    (void)other; 
}

void ilrd::Socket::operator=(const ilrd::Socket &other)
{
    (void)other; 
}

void ilrd::Socket::SocketBind(int port)
{
    int yes = 1; 

    if (-1 == setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes))
    {
        throw std::runtime_error("Failed to setsockopt");
    }

    sockaddr_in servaddr; 
    memset(&servaddr, 0, sizeof(struct sockaddr_in)); 

    servaddr.sin_family = m_socket_domain; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    if(-1 == bind(m_fd, (struct sockaddr*)&servaddr, sizeof(sockaddr_in)))
    {
        throw std::runtime_error("Failed to bind");
    }
}

void ilrd::Socket::CreateSockAdrr(sockaddr_in* destAddr, const char* buf, int port)
{
    memset(destAddr, 0, sizeof(struct sockaddr_in)); 

    destAddr->sin_family = m_socket_domain; 
    destAddr->sin_port = htons(port); 

    if(NULL == buf)
    {
        destAddr->sin_addr.s_addr = htonl(INADDR_BROADCAST); 
        return; 
    }
    
    if( 1 != inet_pton(m_socket_domain, buf, &destAddr->sin_addr))
    {
        throw std::runtime_error("Failed to create socket address");
    }
}