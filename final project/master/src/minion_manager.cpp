/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <iostream>
#include <thread>
#include <cstdint>
#include <string>
#include <cstring>

#include "logger.hpp"

#include "minion_manager.hpp"
#include "definitions.hpp"


const int ilrd::MinionManager::m_no_minions = 3; 
const int ilrd::MinionManager::m_minion_block_size = 4*1024*1024; 
const int ilrd::MinionManager::m_total_block_size = m_minion_block_size*2; 
const std::string ilrd::MinionManager::ips[3] = {"10.10.0.175", "10.10.0.175", "10.10.2.25"}; 

ilrd::LinuxListener ilrd::MinionManager::m_listener;
ilrd::Reactor ilrd::MinionManager::m_reactor(MinionManager::m_listener);


void ilrd::MinionManager::Init()
{

    for(size_t i = 0; i < m_no_minions; ++i)
    {
        std::shared_ptr<char> ptr(new char[ips[i].length()]);
        memcpy(ptr.get(), ips[i].c_str(), ips[i].length()); 
        m_minions.emplace_back(std::shared_ptr<ilrd::MinionProxy>(new MinionProxy(12345 + i, ptr)));
        m_reactor.Register(m_minions[i]->GetMinionFD(), ilrd::enums::mode::READ, [this, i](int fd, ilrd::enums::mode mode)
        {
            this->m_minions[i]->OnMinionFDWakeup(fd, mode);
        });
    }

    m_reactor_thread = std::jthread(&ilrd::MinionManager::ThreadHandle, this);
}

void ilrd::MinionManager::ThreadHandle()
{
    m_reactor.Run(); //maybe reactor should be on framework
}

void ilrd::MinionManager::AddReadTask(uint64_t offset, uint32_t length, ilrd::Uid uid)
{
    //add task to the right minion proxy
    size_t main_minion_index = offset / m_minion_block_size; 
    size_t main_minion_offset = offset % m_minion_block_size;
    size_t backup_minion_index = (main_minion_index + 1) % m_no_minions;
    size_t backup_minion_offset = m_minion_block_size + main_minion_offset;
    std::shared_ptr<ilrd::Ticket> ticket(new Ticket(uid, {m_minions[main_minion_index], m_minions[backup_minion_index]})); 
    //notify with the dispatcher
    m_dispatcher_new_ticket.Notify(ticket);

    //create ticket eith the minion proxy
    //add tasks
    m_minions[main_minion_index]->AddReadTask(main_minion_offset, length, uid, 
        [ticket](ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf)
        {
            ticket->OnMinionProxyDone(status, len, buf); //???
        });  // what is the callback
    m_minions[backup_minion_index]->AddReadTask(backup_minion_offset, length, uid, 
        [ticket](ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf)
        {
            ticket->OnMinionProxyDone(status, len, buf); //???
        });  // what is the callback
    
    std::string lod_data = "New REad task for main minion: " + std::to_string(main_minion_index + 1)
        + ". offset: " +  std::to_string(main_minion_offset) + ", len " +  std::to_string(length); 

    std::string lod_data2 = "New Read task for backup minion: " + std::to_string(backup_minion_index + 1)
        + ". offset: " +  std::to_string(backup_minion_offset) + ", len " +  std::to_string(length);  

    ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data, ilrd::Logger::sevirity::INFO); 
    ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data2, ilrd::Logger::sevirity::INFO); 
}

void ilrd::MinionManager::AddWriteTask(uint64_t offset, uint32_t length, ilrd::Uid uid, std::shared_ptr<char[]> data)
{
    //add task to the right minion proxy
    size_t main_minion_index = offset / m_minion_block_size; 
    size_t main_minion_offset = offset % m_minion_block_size;
    size_t backup_minion_index = (main_minion_index + 1) % m_no_minions;
    size_t backup_minion_offset = m_minion_block_size + main_minion_offset;
    std::shared_ptr<ilrd::Ticket> ticket(new Ticket(uid, {m_minions[main_minion_index], m_minions[backup_minion_index]})); 

    //notify with the dispatcher
    m_dispatcher_new_ticket.Notify(ticket);

    //create ticket eith the minion proxy
    //add tasks
    m_minions[main_minion_index]->AddWriteTask(main_minion_offset, length, data, uid, 
        [ticket](ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf)
        {
            ticket->OnMinionProxyDone(status, len, buf); 
        });

    m_minions[backup_minion_index]->AddWriteTask(backup_minion_offset, length, data, uid, 
        [ticket](ilrd::enums::status status, uint64_t len, std::shared_ptr<char[]> buf)
        {
            ticket->OnMinionProxyDone(status, len, buf); 
        });
    std::string lod_data = "New Write task for main minion: " + std::to_string(main_minion_index + 1)
                                + ". offset: " +  std::to_string(main_minion_offset) + ", len " +  std::to_string(length); 

    std::string lod_data2 = "New Write task for backup minion: " + std::to_string(backup_minion_index + 1)
                                + ". offset: " +  std::to_string(backup_minion_offset) + ", len " +  std::to_string(length); 

    ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data, ilrd::Logger::sevirity::INFO); 
    ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data2, ilrd::Logger::sevirity::INFO); 
    
}

void ilrd::MinionManager::RegisterForNewTickets(ilrd::Acallback<std::shared_ptr<Ticket>>* obj)
{
    m_dispatcher_new_ticket.Register(obj); 
}