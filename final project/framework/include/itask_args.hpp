/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#ifndef ILRD_RD1645_ITASK_ARGS_HPP
#define ILRD_RD1645_ITASK_ARGS_HPP


namespace ilrd
{
class ITaskArgs
{
public:
    virtual int GetKey() = 0;
};
} //ilrd

#endif //ILRD_RD1645_ITASK_ARGS_HPP