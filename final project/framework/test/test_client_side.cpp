/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 23.3.25
Exercise: Reactor
***/

//gdpp23 test/test_client_side.cpp ../../sockets/src/udp.cpp ../../sockets/src/socket.cpp -I./include -I../../sockets/include -o test_client

#include <fcntl.h>
#include <iostream>
#include <reactor.hpp>
#include <udp.hpp>
#include <thread>

static int server_port = 1026;
static  char srv_ip[10] = "127.0.1.1";
static int counter = 0; 

ilrd::Udp new_socket(server_port, 0); 

void ClientSide()
{

    while (counter < 10)
    {
        const char* ping  = "ping"; 
        new_socket.ClientSend(ping, 4, srv_ip, server_port); 
        std::cout << "client side" << std::endl;
        ++counter; 

    }
}

int main()
{
   // g_reactor.Register(new_socket.GetSocketFD(), ilrd::IListener::WRITE, ClientSide); 
   ClientSide(); 

    return 0; 
}