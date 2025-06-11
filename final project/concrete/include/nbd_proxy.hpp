/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_NBD_PROXY_HPP
#define ILRD_RD1645_NBD_PROXY_HPP

#include <map>
#include <linux/nbd.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>

#include "input_proxy.hpp"
#include "uid.hpp"
#include "task_args.hpp"
#include "dispatcher.hpp"
#include "definitions.hpp"

namespace ilrd
{
class NBDProxy : public IInputProxy
{
public:
    NBDProxy();
    std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, ilrd::enums::mode mode); 
    void RegisterForNewTaskArgs(Acallback<std::shared_ptr<ATaskArgs>>* obj);
    void NBDReadResponse(ilrd::Uid uid, ilrd::enums::status status, std::shared_ptr<char[]> buffer, uint64_t len); 
    void NBDWriteResponse(ilrd::Uid uid, ilrd::enums::status status);

private:
    ilrd::Dispatcher<std::shared_ptr<ATaskArgs>> m_dispatcher;
    int m_fd; 
    std::map<ilrd::Uid, struct nbd_reply> m_uid_nbdreply_map; 
    static int read_all(int fd, char* buf, size_t count);
    static int write_all(int fd, char* buf, size_t count);

};
}

#endif //ILRD_RD1645_NBD_PROXY_HPP