/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Minion part
***/

#ifndef ILRD_RD1645_MINION_COMMAND_HPP
#define ILRD_RD1645_MINION_COMMAND_HPP

#include <memory>
#include "icommand.hpp"
#include "itask_args.hpp"

namespace ilrd
{
class MinionCommandRead : public ICommand
{
public:
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args);
};

class MinionCommandWrite : public ICommand
{
public:
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args);
}; 
}//ilrd

#endif //ILRD_RD1645_MINION_COMMAND_HPP