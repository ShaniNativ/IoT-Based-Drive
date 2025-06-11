/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <assert.h> /* assert */
#include <unistd.h> /* pid */
#include <stdio.h> /* printf */
#include <stdlib.h> /* system */
#include <string.h> /* memset */
#include <arpa/inet.h> /*struct socket_ in */
#include <ifaddrs.h> /*getifaddres*/
#include <stdexcept>

#include <iostream>
#include "uid.hpp"

std::atomic_size_t ilrd::Uid::m_atomic_counter = {0};

ilrd::Uid::Uid() : m_time(time(NULL)), m_process_id(getpid()), m_counter(++m_atomic_counter)
{
	if(SetIP())
	{
		throw std::runtime_error("Failed to init IP");
	}
}

bool ilrd::Uid::operator<(const Uid& other) const
{
    return m_counter < other.m_counter || m_time < other.m_time; 
}

bool ilrd::Uid::operator==(const Uid &other) const
{
    return  (m_counter == other.m_counter) && (m_process_id == other.m_process_id) && (m_time == other.m_time)
			&& (m_ip[0] == other.m_ip[0] &&  m_ip[1] == other.m_ip[1] && m_ip[2] == other.m_ip[2] && m_ip[3] == other.m_ip[3]); 
}

uint64_t ilrd::Uid::GetSize()
{
    return sizeof(time_t) + sizeof(pid_t) + sizeof(size_t) + m_ip_size;
}

char *ilrd::Uid::ToBuffer(char *buffer)
{
	*(reinterpret_cast<time_t*>(buffer)) = m_time; 
	buffer += sizeof(time_t); 

	*(reinterpret_cast<pid_t*>(buffer)) = m_process_id; 
	buffer += sizeof(pid_t); 

	*(reinterpret_cast<size_t*>(buffer)) = m_counter; 
	buffer += sizeof(size_t); 

	memcpy(buffer, m_ip, m_ip_size); 

	buffer += m_ip_size; 

    return buffer;

}

char *ilrd::Uid::FromBuffer(char *buffer)
{
	m_time = *(reinterpret_cast<time_t*>(buffer)); 
	buffer += sizeof(time_t); 

	m_process_id = *(reinterpret_cast<pid_t*>(buffer)); 
	buffer += sizeof(pid_t); 

	m_counter = *(reinterpret_cast<size_t*>(buffer)); 
	buffer += sizeof(size_t); 

	memcpy(m_ip, buffer, m_ip_size); 

	buffer += m_ip_size; 

    return buffer;
}

bool ilrd::Uid::SetIP()
{
    struct ifaddrs* ifaddrs = NULL; 
	struct ifaddrs* ifcurrent = NULL;
	struct sockaddr_in* saddr = NULL; 
	int flag = -1;
	
	if (-1 == getifaddrs(&ifaddrs))
	{
	   return 1;
	}
	
	for(ifcurrent = ifaddrs; ifcurrent != NULL; ifcurrent = ifcurrent->ifa_next)
	{
		if(NULL != ifcurrent->ifa_addr && ifcurrent->ifa_addr->sa_family == AF_INET)
		{
			if(0 != strcmp(ifcurrent->ifa_name, "lo"))
			{
				saddr = (struct sockaddr_in*)ifcurrent->ifa_addr; 
				memcpy(m_ip, &saddr->sin_addr, m_ip_size);
				flag = 0; 
				break; 
			}
		}
	}
	
	freeifaddrs(ifaddrs);
	
	return flag == 0 ? 0 : 1;
}


void ilrd::Uid::PrintUID()
{
	std::cout << "Uid:" << m_counter << " " << m_time << " " << m_process_id << " " 
	<< static_cast<int>(m_ip[0]) << "." << static_cast<int>(m_ip[1]) << "." << static_cast<int>(m_ip[2]) << "." 
	<< static_cast<int>(m_ip[3]) <<  std::endl; 
}
