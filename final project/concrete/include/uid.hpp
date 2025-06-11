/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_UID_HPP
#define ILRD_RD1645_UID_HPP

#include <atomic>


namespace ilrd
{

static const size_t m_ip_size = 4;

class Uid
{
public:
    Uid(); 
    bool operator<(const Uid& other) const; 
    bool operator==(const Uid& other) const; 
    uint64_t GetSize(); 
    char* ToBuffer(char* buffer); 
    char* FromBuffer(char* buffer); 
    void PrintUID();

private:
    time_t m_time;
    pid_t m_process_id;
    size_t m_counter; 
    unsigned char m_ip[m_ip_size];
    bool SetIP(); 
    static std::atomic<size_t> m_atomic_counter;

};
}


#endif //ILRD_RD1645_UID_HPP