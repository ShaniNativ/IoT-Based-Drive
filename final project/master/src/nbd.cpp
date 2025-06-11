/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - NBD 
***/
#define _POSIX_C_SOURCE (200809L)

#include <iostream>


#include <assert.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "nbd.hpp"

const size_t ilrd::NBD::m_nbd_size = 8*1024*1024; 

/* Signal handler to gracefully disconnect from nbd kernel driver. */
int ilrd::NBD::nbd_dev_to_disconnect = -1;

void ilrd::NBD::disconnect_nbd(int signal) 
{
    (void)signal;
    if (nbd_dev_to_disconnect != -1) 
    {
        if(ioctl(nbd_dev_to_disconnect, NBD_DISCONNECT) == -1) 
        {
            warn("failed to request disconect on nbd device");
        } 
        else 
        {
            nbd_dev_to_disconnect = -1;
            fprintf(stderr, "sucessfuly requested disconnect on nbd device\n");
        }
    }
}

/* Sets signal action like regular sigaction but is suspicious. */
int ilrd::NBD::set_sigaction(int sig, const struct sigaction * act) 
{
    struct sigaction oact;
    int r = sigaction(sig, act, &oact);
    if (r == 0 && oact.sa_handler != SIG_DFL) {
        warnx("overriden non-default signal handler (%d: %s)", sig, strsignal(sig));
    }
    return r;
}


int ilrd::NBD::GetNBD_FD()
{
    return m_server_nbd_socket;
}


ilrd::NBD::NBD(std::string device_file)  : m_device_file(device_file)
{                      

}

int ilrd::NBD::Init()
{
    // 1. Init device:
    int nbd_fd = open(m_device_file.c_str(), O_RDWR);

    if(-1 == nbd_fd)
    {
        std::cout << "Failed to open " << m_device_file.c_str() << std::endl; 
    }
    ioctl(nbd_fd, NBD_SET_SIZE, m_nbd_size); // dev_size must be divisible by 4096 
    ioctl(nbd_fd, NBD_CLEAR_QUE);
    ioctl(nbd_fd, NBD_CLEAR_SOCK);

    // 2. Init server and client processes to receive NBD messages:
    // - Create connected sockets to be used by server and client:
    int sp[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
    // - Create client: Child process
    pid_t pid = fork();
    if (0 == pid)
    {

        close(sp[0]);
        m_client_nbd_socket = sp[1];
        ioctl(nbd_fd, NBD_SET_SOCK, sp[1]);
        // NBD_DO_IT: From now on NBD messages will be sent 
        // by sp[1], and may be read by sp[0]:
        ioctl(nbd_fd, NBD_DO_IT);
        // Now blocked. When done, cleanup and exit:
        ioctl(nbd_fd, NBD_CLEAR_QUE);
        ioctl(nbd_fd, NBD_CLEAR_SOCK); 
        return 0;
    }

    // - Client: Continue in parent process
    close(sp[1]);
    m_server_nbd_socket = sp[0]; 

    // 3. Wait for NBD requests, handle them and reply:
    // while (true)
    // {
    //     select(sp[0],0/*READ*/);
    //     OnNBDMessage(sp[0]);
    // }

	// Cleanup and closures
	// (In this pseudo we don't ever get here)
	// ...
	
	return 0;
}

