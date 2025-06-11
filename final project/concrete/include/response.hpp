/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_RESPONSE_HPP
#define ILRD_RD1645_RESPONSE_HPP

#include "definitions.hpp"
#include "uid.hpp"
#include "nbd_proxy.hpp"
#include <memory>

namespace ilrd
{

class IResponse
{
public:
    virtual void OnSuccess(ilrd::NBDProxy* nbd_proxy, Uid uid, std::shared_ptr<char[]> buffer, uint64_t len)= 0; 
    virtual void OnFailure(ilrd::NBDProxy* nbd_proxy, Uid uid)= 0; 
};

class ReadResponse : public IResponse
{
public:
    void OnSuccess(ilrd::NBDProxy* nbd_proxy, Uid uid, std::shared_ptr<char[]> buffer, uint64_t len);
    void OnFailure(ilrd::NBDProxy* nbd_proxy, Uid uid);
};

class WriteResponse : public IResponse
{
public:
    void OnSuccess(ilrd::NBDProxy* nbd_proxy, Uid uid, std::shared_ptr<char[]> buffer, uint64_t len);
    void OnFailure(ilrd::NBDProxy* nbd_proxy, Uid uid);
};



}

#endif //ILRD_RD1645_RESPONSE_HPP