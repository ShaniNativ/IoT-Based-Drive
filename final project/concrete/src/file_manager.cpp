/***
Written by: Shani Davidian
Reviewed by:
Date: 20.4.25
Exercise: Final Project - Minion part
***/
#include <iostream>

#include "file_manager.hpp"

ilrd::FileManager::FileManager()
{
    
}

void ilrd::FileManager::Init(const char* drive_name)

{
    m_drive.open(drive_name, std::fstream::in | std::fstream::out);
    // truncate(drive_name, 8 * 1024 * 1024);
}

ilrd::FileManager::~FileManager()
{
    m_drive.close();
}

ilrd::enums::status ilrd::FileManager::Read(uint64_t len, uint64_t offset, std::shared_ptr<char[]> buffer)
{
    std::unique_lock<std::mutex> lock(m_write_mutex);
    try
    {
        m_drive.seekp(offset);
        m_drive.read(buffer.get(), len);
    }
    catch (const std::exception &e)
    {
        return ilrd::enums::status::FAILURE;
    }

    std::cout << "FileManager read from file: len: " << len << " offset: " << offset << " data: ";
    for (size_t i = 0; i < len; ++i)
    {
        std::cout << buffer.get()[i];
    }

    std::cout << std::endl;

    return ilrd::enums::status::SUCCESS;
}

ilrd::enums::status ilrd::FileManager::Write(uint64_t len, uint64_t offset, std::shared_ptr<char[]> data_to_write)
{
    std::unique_lock<std::mutex> lock(m_write_mutex);
    try
    {
        m_drive.seekp(offset);
        m_drive.write(data_to_write.get(), len);
    }
    catch (const std::exception &e)
    {
        return ilrd::enums::status::FAILURE;
    }

    m_drive.flush();

    std::cout << "FileManager Write to file: len: " << len << " offset: " << offset << " data: ";
    for (size_t i = 0; i < len; ++i)
    {
        std::cout << data_to_write.get()[i];
    }
    std::cout << std::endl;

    return ilrd::enums::status::SUCCESS;
}
