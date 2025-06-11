/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_TASK_ARGS_HPP
#define ILRD_RD1645_TASK_ARGS_HPP

#include <memory>

#include "itask_args.hpp"
#include "uid.hpp"

namespace ilrd
{

class ATaskArgs : public ITaskArgs
{
public:
    enum key  
    {
        READ = 0, 
        WRITE = 1
    }; 
    ATaskArgs(); 
    ilrd::Uid GetUID();  // to wrapp uid in a class
    void SetUID(ilrd::Uid uid); 
private:
    ilrd::Uid m_uid; 
    
};

class ReadArgs : public ATaskArgs
{
public:
    ReadArgs(uint64_t offset, uint32_t len); 
    int GetKey();
    uint64_t GetOffset();
    uint32_t GetLength();
    void SetOffset(uint64_t offset);
    void SetLength(uint32_t length);

private:
    uint64_t m_offset; 
    uint32_t m_length; 
}; 

class WriteArgs : public ATaskArgs
{
public:
    WriteArgs(uint64_t offset, uint32_t len);
    int GetKey();
    uint64_t GetOffset();
    uint32_t GetLength();
    void SetOffset(uint64_t offset);
    void SetLength(uint32_t len);
    std::shared_ptr<char[]>  GetBuffer();
    void  SetBuffer(const char* str);

private:
    uint64_t m_offset; 
    uint32_t m_length; 
    std::shared_ptr<char[]> m_buffer; 

}; 

} //ilrd



#endif //ILRD_RD1645_TASK_ARGS_HPP
