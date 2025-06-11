/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_COMMAND_HPP
#define ILRD_RD1645_COMMAND_HPP

#include <memory>
#include "icommand.hpp"
#include "itask_args.hpp"
#include "uid.hpp"


namespace ilrd
{
class CommandRead : public ICommand
{
public:
    CommandRead(); 
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args);

};
class CommandWrite : public ICommand
{
public:
    CommandWrite(); 
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args);
}; 


class ErrorHandling
{
public:
    ErrorHandling(Uid uid); 
    bool operator()(); 
private:
    int m_async_injection_calls; 
    Uid m_uid; 

}; 

}//ilrd
#endif //ILRD_RD1645_COMMAND_HPP