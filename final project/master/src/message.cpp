/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/


#include <cstring>
#include <iostream>
#include "message.hpp"



/*=================================== AMessage ===============================*/

ilrd::AMessage::AMessage(ilrd::Uid uid) : m_uid(uid)
{
}

uint64_t ilrd::AMessage::GetSize()
{
    return m_uid.GetSize() + sizeof(uint64_t) + sizeof(enums::message_type)  ; //class_type; 
}

char* ilrd::AMessage::ToBuffer(char* buffer) // char is promotoed to the last write location
{
    *(reinterpret_cast<uint64_t*>(buffer)) = GetSize();
    buffer += sizeof(uint64_t);

    *(reinterpret_cast<ilrd::enums::message_type*>(buffer)) = GetClassType();
    buffer += sizeof(enums::message_type);

    buffer = m_uid.ToBuffer(buffer);

    return buffer;
}

char* ilrd::AMessage::FromBuffer(char* buffer)
{
    //on recieve there is a read from fd: read of size & class type 
    // uint64_t size = *(uint64_t*)buffer;
    // buffer += sizeof(uint64_t);

    // ilrd::enums::message_type class_type = *(ilrd::enums::message_type*)buffer;
    // buffer += sizeof(ilrd::enums::message_type);

    buffer = m_uid.FromBuffer(buffer); 

    return buffer;
}
ilrd::Uid ilrd::AMessage::GetUid()
{
    return m_uid;
}

/*=================================== AMessage Request  ===============================*/

ilrd::AMessageRequest::AMessageRequest(ilrd::Uid uid, uint32_t len, uint64_t offset)
                                : AMessage(uid), m_len(len), m_offset(offset)
{
}

uint64_t ilrd::AMessageRequest::GetSize()
{
    return AMessage::GetSize() + sizeof(m_len) + sizeof(m_offset);
}

uint32_t ilrd::AMessageRequest::GetLength()
{
    return m_len;
}

uint64_t ilrd::AMessageRequest::GetOffset()
{
    return m_offset;
}

char* ilrd::AMessageRequest::ToBuffer(char* buffer)
{
    buffer = AMessage::ToBuffer(buffer); 
    *(reinterpret_cast<uint32_t*>(buffer)) = m_len; 
    buffer += sizeof(uint32_t);
    *(reinterpret_cast<uint64_t*>(buffer)) = m_offset; 
    buffer += sizeof(uint64_t);

    return buffer;
}

char* ilrd::AMessageRequest::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer); 
    m_len = *(reinterpret_cast<uint32_t*>(buffer)); 
    buffer += sizeof(uint32_t);
    m_offset = *(reinterpret_cast<uint64_t*>(buffer)); 
    buffer += sizeof(uint64_t);

    return buffer;
}

/*=================================== MessageReadRequest ====================*/

ilrd::MessageReadRequest::MessageReadRequest(ilrd::Uid uid, uint32_t len, uint64_t offset) : AMessageRequest(uid, len, offset)
{

}

ilrd::enums::message_type ilrd::MessageReadRequest::GetClassType()
{
    return (ilrd::enums::message_type::READ_REQUEST); //need the cast??
}

uint64_t ilrd::MessageReadRequest::GetSize()
{

    return AMessageRequest::GetSize();
}

/*=================================== MessageWriteRequest ====================*/

ilrd::MessageWriteRequest::MessageWriteRequest(ilrd::Uid uid, uint32_t len, uint64_t offset, 
                std::shared_ptr<char[]> data) : AMessageRequest(uid, len, offset), m_data(data)
{
}

uint64_t ilrd::MessageWriteRequest::GetSize()
{
    return AMessageRequest::GetSize()  + AMessageRequest::GetLength(); 
}

ilrd::enums::message_type ilrd::MessageWriteRequest::GetClassType()
{
    return (ilrd::enums::message_type::WRITE_REQUEST); //need the cast??
}

std::shared_ptr<char[]> ilrd::MessageWriteRequest::GetData()
{
    return m_data;
}

char* ilrd::MessageWriteRequest::ToBuffer(char* buffer)
{
    buffer = AMessageRequest::ToBuffer(buffer); 
    memcpy(buffer, m_data.get(), AMessageRequest::GetLength()); 
    buffer += AMessageRequest::GetLength(); 

    return buffer; 
}

char* ilrd::MessageWriteRequest::FromBuffer(char* buffer)
{
    buffer = AMessageRequest::FromBuffer(buffer); 
    m_data = std::shared_ptr<char[]>(new char[AMessageRequest::GetLength()]); 
    memcpy(m_data.get(),buffer, AMessageRequest::GetLength()); 
    buffer += AMessageRequest::GetLength(); 

    return buffer;
}

/*=================================== Message Response ====================*/

ilrd::AMessageResponse::AMessageResponse(ilrd::Uid uid, ilrd::enums::status status) : AMessage(uid), m_status(status)
{
}

uint64_t ilrd::AMessageResponse::GetSize()
{
    return AMessage::GetSize() + sizeof(ilrd::enums::status);
}

ilrd::enums::status ilrd::AMessageResponse::GetStatus()
{
    return m_status;
}

uint32_t ilrd::AMessageResponse::GetLength()
{
    return 0;
}

std::shared_ptr<char[]> ilrd::AMessageResponse::GetBuffer()
{
    return nullptr;
}

char *ilrd::AMessageResponse::ToBuffer(char* buffer)
{
    buffer = AMessage::ToBuffer(buffer); 
    *(reinterpret_cast<ilrd::enums::status*>(buffer)) = m_status; 
    buffer += sizeof(ilrd::enums::status);

    return buffer;
}

char *ilrd::AMessageResponse::FromBuffer(char* buffer)
{
    buffer = AMessage::FromBuffer(buffer); 
    m_status = *(reinterpret_cast<ilrd::enums::status*>(buffer)); 
    buffer += sizeof(ilrd::enums::status);

    return buffer;
}

/*=================================== MessageReadResponse ====================*/

ilrd::MessageReadResponse::MessageReadResponse(ilrd::Uid uid, ilrd::enums::status status, uint32_t len, std::shared_ptr<char[]> read_data) :
                                        AMessageResponse(uid, status), m_len(len), m_output_buffer(read_data)
{
}

uint64_t ilrd::MessageReadResponse::GetSize()
{
    return AMessageResponse::GetSize()  + sizeof(m_len) + m_len; 
}
ilrd::enums::message_type ilrd::MessageReadResponse::GetClassType()
{
    return (ilrd::enums::message_type::READ_RESPONSE);
}

uint32_t ilrd::MessageReadResponse::GetLength()
{
    return m_len;
}

std::shared_ptr<char[]> ilrd::MessageReadResponse::GetBuffer()
{
    return m_output_buffer;
}

char* ilrd::MessageReadResponse::ToBuffer(char* buffer)
{
    buffer = AMessageResponse::ToBuffer(buffer); 
    *(reinterpret_cast<uint32_t*>(buffer)) = m_len; 
    buffer += sizeof(uint32_t);
    memcpy(buffer, m_output_buffer.get(), m_len); 
    buffer += m_len; 

    return buffer;
}

char* ilrd::MessageReadResponse::FromBuffer(char* buffer)
{
    buffer = AMessageResponse::FromBuffer(buffer); 
    m_len = *(reinterpret_cast<uint32_t*>(buffer)); 
    buffer += sizeof(uint32_t);
    m_output_buffer = std::shared_ptr<char[]>(new char[m_len]); 
    memcpy(m_output_buffer.get(), buffer, m_len); 
    buffer += m_len; 

    return buffer;
}

/*=================================== MessageWriteResponse ====================*/

ilrd::MessageWriteResponse::MessageWriteResponse(ilrd::Uid uid, ilrd::enums::status status) : AMessageResponse(uid, status)
{
}


ilrd::enums::message_type ilrd::MessageWriteResponse::GetClassType()
{
    return (ilrd::enums::message_type::WRITE_RESPONSE); //need the cast??
}


