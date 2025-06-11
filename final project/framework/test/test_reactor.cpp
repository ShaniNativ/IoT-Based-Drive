/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 23.3.25
Exercise: Reactor
***/

//gdpp23 test/test_reactor.cpp src/reactor.cpp src/listener.cpp ../../sockets/src/udp.cpp ../../sockets/src/socket.cpp -I./include -I../../sockets/include 

#include <fcntl.h>
#include <iostream>
#include <reactor.hpp>
#include <udp.hpp>
#include <thread>

static ilrd::LinuxListener g_listener; 
static ilrd::Reactor g_reactor(g_listener); 


static int server_port = 1026;
// static  char srv_ip[10] = "127.0.1.1";
// static int counter = 0; 

ilrd::Udp new_socket(server_port, 1); 

void ServerSide(int fd, ilrd::IListener::mode mode)
{
    (void)fd; 
    (void)mode; 
    char recieved_from_client[5] = {0}; 
    new_socket.SocketRcv(recieved_from_client, 5); 
    std::cout << recieved_from_client << std::endl;
}

static void Stop(int fd, ilrd::IListener::mode mode)
{
    (void)mode; 
    char buf[10] = {0};
    read(fd, buf, 5);
    g_reactor.Stop(); 
}

int main()
{
    g_reactor.Register(new_socket.GetSocketFD(), ilrd::IListener::READ, ServerSide); 
    g_reactor.Register(STDIN_FILENO, ilrd::IListener::READ, Stop); 

   // std::jthread thread(ClientSide, new_socket.GetSocketFD(),ilrd::IListener::WRITE);
   // new_socket.ClientSend("ping", 5, srv_ip, server_port); 
    g_reactor.Run(); 

    return 0; 
}

