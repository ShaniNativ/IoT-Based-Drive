/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#ifndef ILRD_RD1645_ICOMMAND_HPP
#define ILRD_RD1645_ICOMMAND_HPP

#include <functional> /*std::pair, std::function*/
#include <chrono> /* std::chrono */

#include "itask_args.hpp"

namespace ilrd
{
class ICommand
{
public:
    virtual std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args) = 0;
};
}//ilrd


#endif //ILRD_RD1645_ICOMMAND_HPP
