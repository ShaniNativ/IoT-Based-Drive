/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - NBD 
***/

#ifndef ILRD_RD1645_NBD_HPP
#define ILRD_RD1645_NBD_HPP

#include <memory>


//#include "tcp_client.hpp"

namespace ilrd
{

class NBD
{
public:
    NBD(std::string device_file); 
    int Init();
    int GetNBD_FD(); 
    static int set_sigaction(int sig, const struct sigaction * act); 
    static void disconnect_nbd(int signal); 
   
private:
    std::string m_device_file; 
    int m_client_nbd_socket; 
    int m_server_nbd_socket; 
    const static size_t m_nbd_size; 
    static int nbd_dev_to_disconnect; 

};

}

#endif //ILRD_RD1645_NBD_HPP