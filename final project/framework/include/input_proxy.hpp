/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#ifndef ILRD_RD1645_INPUT_PROXY_HPP
#define ILRD_RD1645_INPUT_PROXY_HPP

#include <memory>

#include "definitions.hpp"
#include "listener.hpp"
#include "itask_args.hpp"

namespace ilrd
{
class IInputProxy
{
public:
    virtual std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, ilrd::enums::mode mode) = 0;
    virtual ~IInputProxy() = default;
};
}


#endif //ILRD_RD1645_INPUT_PROXY_HPP
