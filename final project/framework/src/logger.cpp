/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Logger
***/

#include <iomanip>

#include "logger.hpp"


void ilrd::Logger::ThreadWork()
{
    while(m_running || !m_log_data_queue.Empty())
    {
        ilrd::Logger::LogData temp;
        m_log_data_queue.Pop(temp);
        std::tm tm = *std::localtime(&temp.m_time);
        m_log_file << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")<< " " << 
        m_sevirity_lut[temp.m_sevirity] << " " << temp.m_data << std::endl;
    } 
}

ilrd::Logger::Logger(const std::string& file_name) :m_log_file(file_name), m_running(true), 
        m_sevirity_lut{"DEBUG", "WARNING", "ERROR", "INFO"}, 
         m_worker_thread(&ilrd::Logger::ThreadWork, this)                                 
{
    
}

ilrd::Logger::~Logger()
{
    m_running = false; 
    ilrd::Logger::Log("End of Log File", INFO);
    m_worker_thread.join();
}

void ilrd::Logger::Log(const std::string& str, ilrd::Logger::sevirity sevirity)
{
    ilrd::Logger::LogData temp = {str, sevirity, std::time(0)}; 
    m_log_data_queue.Push(temp); 
}