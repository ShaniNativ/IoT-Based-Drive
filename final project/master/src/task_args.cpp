/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <cstring>
#include "task_args.hpp"

/*============================= Task Args ===================================*/

ilrd::ATaskArgs::ATaskArgs() : m_uid(Uid())
{

}

ilrd::Uid ilrd::ATaskArgs::GetUID()
{
    return m_uid;
}

void ilrd::ATaskArgs::SetUID(ilrd::Uid uid)
{
    m_uid = uid; 
}

/*============================= Read Args ===================================*/

ilrd::ReadArgs::ReadArgs(uint64_t offset, uint32_t len) : m_offset(offset), m_length(len)
{

}

int ilrd::ReadArgs::GetKey()
{
    return key::READ; 
}

uint64_t ilrd::ReadArgs::GetOffset()
{
    return m_offset; 
}

uint32_t ilrd::ReadArgs::GetLength()
{
    return m_length;
}

void ilrd::ReadArgs::SetOffset(uint64_t offset)
{
    m_offset = offset; 
}

void ilrd::ReadArgs::SetLength(uint32_t length)
{
    m_length = length;
}

/*=========================== Write Args =====================================*/

ilrd::WriteArgs::WriteArgs(uint64_t offset, uint32_t len)  : m_offset(offset), m_length(len), m_buffer(new char[len])
{

}

int ilrd::WriteArgs::GetKey()
{
    return key::WRITE;
}

uint64_t ilrd::WriteArgs::GetOffset()
{
    return m_offset; 
}

uint32_t ilrd::WriteArgs::GetLength()
{
    return m_length;
}

void ilrd::WriteArgs::SetOffset(uint64_t offset)
{
     m_offset = offset; 
}

void ilrd::WriteArgs::SetLength(uint32_t len)
{
     m_length = len;
}

std::shared_ptr<char[]>  ilrd::WriteArgs::GetBuffer()
{
    return m_buffer;
}

void  ilrd::WriteArgs::SetBuffer(const char* str)
{
    memcpy(m_buffer.get(), str, m_length); 
}