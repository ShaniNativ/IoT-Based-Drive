/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#ifndef ILRD_RD1645_FW_TPTASK_HPP
#define ILRD_RD1645_FW_TPTASK_HPP

#include "thread_pool.hpp"
#include "itask_args.hpp"

namespace ilrd
{
namespace thread_pool
{

class FW_TPTask : public ITPTask
{
public:
    FW_TPTask(std::shared_ptr<ITaskArgs> args);
    void Run(); 
private:
    std::shared_ptr<ITaskArgs> m_p_tasks_args; 

};

} //thread_pool
}//ilrd

#endif //ILRD_RD1645_FW_TPTASK_HPP

