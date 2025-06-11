/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/

#ifndef ILRD_RD1645_FILE_MANAGER_HPP
#define ILRD_RD1645_FILE_MANAGER_HPP

#include <fstream>
#include <mutex>

#include "ifile_manager.hpp"
#include "handleton.hpp"


namespace ilrd
{
class FileManager : public IFileManager
{
public:
    void Init(const char* drive_name); 
    ~FileManager(); 
    enums::status Read(uint64_t len , uint64_t offset, std::shared_ptr<char[]> buffer); 
    enums::status Write(uint64_t len , uint64_t offset, std::shared_ptr<char[]> data_to_write); 
    friend class Handleton<FileManager>;


private:
    FileManager(); //private
    std::fstream m_drive; 
    std::mutex m_write_mutex; 

};
}

#endif //ILRD_RD1645_IFILE_MANAGER_HPP