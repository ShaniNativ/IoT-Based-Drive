/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/
#ifndef ILRD_RD1645_MESSAGE_HPP
#define ILRD_RD1645_MESSAGE_HPP

#include <cstdint>
#include <memory>

#include "definitions.hpp"
#include "uid.hpp"

namespace ilrd
{

class AMessage
{
public:
    AMessage(ilrd::Uid uid); 
    AMessage() = default; 
    virtual ~AMessage() = default; 
    virtual uint64_t GetSize(); 
    virtual enums::message_type  GetClassType() = 0;
    virtual char* ToBuffer(char* buffer); 
    virtual char* FromBuffer(char* buffer); 
    virtual ilrd::Uid  GetUid(); 

private:
    ilrd::Uid m_uid; 
};

class AMessageRequest : public AMessage
{
public: 
    AMessageRequest(ilrd::Uid uid, uint32_t len , uint64_t offset); 
    AMessageRequest() = default;
    virtual ~AMessageRequest() = default; 
    uint64_t GetSize(); 
    uint32_t GetLength(); 
    uint64_t GetOffset(); 
    char* ToBuffer(char* buffer); 
    char* FromBuffer(char* buffer); 
private:
    uint32_t m_len; 
    uint64_t m_offset; 
}; 

class MessageReadRequest : public AMessageRequest
{
public:
    MessageReadRequest(ilrd::Uid uid, uint32_t len , uint64_t offset); 
    MessageReadRequest() = default; 
    enums::message_type GetClassType();
    uint64_t GetSize(); 
};

class MessageWriteRequest : public AMessageRequest
{
public:
    MessageWriteRequest(ilrd::Uid uid, uint32_t len , uint64_t offset, std::shared_ptr<char[]> data); 
    MessageWriteRequest() = default;
    uint64_t GetSize(); 
    enums::message_type GetClassType();
    std::shared_ptr<char[]> GetData(); 
    char* ToBuffer(char* buffer); 
    char* FromBuffer(char* buffer); 

private:
    std::shared_ptr<char[]> m_data; 

};

class AMessageResponse : public AMessage
{
public:    
    AMessageResponse(ilrd::Uid uid, ilrd::enums::status status); 
    AMessageResponse() = default; 
    uint64_t GetSize();     
    virtual ilrd::enums::status GetStatus();
    virtual uint32_t GetLength(); 
    virtual std::shared_ptr<char[]> GetBuffer(); 
    char* ToBuffer(char* buffer); 
    char* FromBuffer(char* buffer); 
private:
    ilrd::enums::status m_status; 
}; 

class MessageReadResponse : public AMessageResponse
{
public:
    MessageReadResponse(ilrd::Uid uid, ilrd::enums::status status, uint32_t len, std::shared_ptr<char[]> read_data); 
    MessageReadResponse() = default; 
    uint64_t GetSize(); 
    enums::message_type GetClassType();
    virtual uint32_t GetLength(); 
    virtual std::shared_ptr<char[]> GetBuffer(); 
    char* ToBuffer(char* buffer); 
    char* FromBuffer(char* buffer); 

private:
    uint32_t m_len; 
    std::shared_ptr<char[]> m_output_buffer; 
};

class MessageWriteResponse : public AMessageResponse
{
public:
    MessageWriteResponse(ilrd::Uid uid, ilrd::enums::status status);   
    MessageWriteResponse() = default; 
    enums::message_type GetClassType();
};



}//ilrd

#endif //ILRD_RD1645_MESSAGE_HPP
