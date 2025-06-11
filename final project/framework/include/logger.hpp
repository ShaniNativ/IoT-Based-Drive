/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Logger
***/

#ifndef ILRD_RD1645_LOGGER_HPP
#define ILRD_RD1645_LOGGER_HPP

#include <string> /* std::string */
#include <ctime> /* time_t */
#include <fstream> /* ofstream */


#include "waitable_queue.hpp" /* API */
#include "handleton.hpp"

namespace ilrd
{

class Logger
{
    struct LogData;
public:  
    //INFO and NO_OF_SEVERITIY are for internal use only! any use of them may
    //cause undefined behaviour
    enum sevirity {DEBUG, WARNING, ERROR, INFO, NO_OF_SEVERITIY};  
    ~Logger();
    Logger(const Logger& other) = delete; 
    Logger& operator=(const Logger& other) = delete; 
    void Log(const std::string& str, sevirity sevirity); 
    friend ilrd::Handleton<Logger>;
    
private:
    Logger(const std::string& file_name = "log.txt"); //private
    WaitableQueue<ilrd::Logger::LogData> m_log_data_queue;
    std::ofstream m_log_file; 
    bool m_running; 
    std::string m_sevirity_lut[NO_OF_SEVERITIY]; 
    std::thread m_worker_thread;

    void ThreadWork();

    struct LogData
    {
        std::string m_data;
        sevirity m_sevirity; 
        time_t m_time; 
    };

};

}

#endif