/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/
#include <iostream> /* test only */
#include <memory>
#include <assert.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <string.h>

#include "handleton.hpp"
#include "factory.hpp"
#include "nbd_proxy.hpp"
#include "logger.hpp"
#include "handleton.hpp"


#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) {
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) {
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll


int ilrd::NBDProxy::read_all(int fd, char* buf, size_t count)
{
  int bytes_read;

  while (count > 0) {
    bytes_read = read(fd, buf, count);
    assert(bytes_read > 0);
    buf += bytes_read;
    count -= bytes_read;
  }
  assert(count == 0);

  return 0;
}

int ilrd::NBDProxy::write_all(int fd, char* buf, size_t count)
{
  int bytes_written;

  while (count > 0) {
    bytes_written = write(fd, buf, count);
    assert(bytes_written > 0);
    buf += bytes_written;
    count -= bytes_written;
  }
  assert(count == 0);

  return 0;
}

/*================================ NBD PROXY =================================*/
ilrd::NBDProxy::NBDProxy()
{
//needed the factory? 
    // ilrd::Handleton<ilrd::Factory< ilrd::enums::mode, ilrd::ATaskArgs>>::
    //     GetInstace()->Register(ilrd::enums::mode::READ, NewReadArgs);
    // ilrd::Handleton<ilrd::Factory< ilrd::enums::mode, ilrd::ATaskArgs>>::
    //     GetInstace()->Register(ilrd::enums::mode::WRITE, NewWriteArgs);
}

std::shared_ptr<ilrd::ITaskArgs> ilrd::NBDProxy::GetTaskArgs(int fd, ilrd::enums::mode mode)
{
    (void) mode;
    // char buf[BUFSIZ] = {0};
    m_fd = fd; 
    // read(fd, buf, BUFSIZ); 
    struct nbd_request request;
    ssize_t bytes_read = read(m_fd, &request, sizeof(request));
    // Setup reply:
    struct nbd_reply reply;
    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);
    memcpy(reply.handle, request.handle, sizeof(reply.handle));
    // Translate network to host:
    u_int32_t len = ntohl(request.len);
    u_int64_t from = ntohll(request.from);
    // Handle read / write differently:
    std::shared_ptr<ilrd::ATaskArgs> new_args;
    std::string lod_data;
    switch(ntohl(request.type))
    {
    case NBD_CMD_READ:
        new_args = std::make_shared<ilrd::ReadArgs>(ilrd::ReadArgs(from, len)); //from is 64

        lod_data = "NBD Proxy:  new read args:  . offset: " +  std::to_string(from) + ", len " +  std::to_string(len);  

        ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data, ilrd::Logger::sevirity::INFO); 

        break;
    case NBD_CMD_WRITE:
        std::shared_ptr<char[]> chunk = std::shared_ptr<char[]>(new char[len]);
      // read_all() - as implemented in BUSE 
        read_all(m_fd, chunk.get(),len);
        new_args = std::make_shared<ilrd::WriteArgs>(ilrd::WriteArgs(from, len)); //from is 64
        std::static_pointer_cast<ilrd::WriteArgs>(new_args)->SetBuffer(chunk.get());

        lod_data = "NBD Proxy:  new write args:  . offset: " +  std::to_string(from) + ", len " +  std::to_string(len);  

        ilrd::Handleton<ilrd::Logger>::GetInstace()->Log(lod_data, ilrd::Logger::sevirity::INFO);         
        // for (size_t i = 0; i < len; ++i)
        // {
        //     std::cout << chunk.get()[i];
        // }
    
        // std::cout << std::endl;

        break;
    }

    m_uid_nbdreply_map.emplace(new_args->GetUID(), reply);
    m_dispatcher.Notify(std::static_pointer_cast<ATaskArgs>(new_args)); 
    return new_args; 
}

void ilrd::NBDProxy::RegisterForNewTaskArgs(ilrd::Acallback<std::shared_ptr<ilrd::ATaskArgs>>* obj)
{
    m_dispatcher.Register(obj); 

}

void ilrd::NBDProxy::NBDReadResponse(ilrd::Uid uid, ilrd::enums::status status, std::shared_ptr<char[]> buffer, uint64_t len)
{
    struct nbd_reply reply = m_uid_nbdreply_map[uid];
    if(status == ilrd::enums::status::SUCCESS)
    {
      reply.error = htonl(0);
    }
    else
    {
      reply.error = htonl(EPERM);
    }
    write_all(m_fd, (char*)&reply, sizeof(struct nbd_reply));
    write_all(m_fd, buffer.get(), len);
}

void ilrd::NBDProxy::NBDWriteResponse(ilrd::Uid uid, ilrd::enums::status status)
{
    struct nbd_reply reply = m_uid_nbdreply_map[uid];
    if(status == ilrd::enums::status::SUCCESS)
    {
      reply.error = htonl(0);
    }
    else
    {
      reply.error = htonl(EPERM);
    }
    write_all(m_fd, (char*)&reply, sizeof(struct nbd_reply));
}
