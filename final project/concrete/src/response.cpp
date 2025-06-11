/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#include "response.hpp"



void ilrd::ReadResponse::OnSuccess(ilrd::NBDProxy* nbd_proxy, Uid uid, std::shared_ptr<char[]> buffer, uint64_t len)
{
    nbd_proxy->NBDReadResponse(uid, ilrd::enums::status::SUCCESS, buffer, len);
}

void ilrd::ReadResponse::OnFailure(ilrd::NBDProxy *nbd_proxy, Uid uid)
{
    nbd_proxy->NBDReadResponse(uid, ilrd::enums::status::FAILURE, nullptr, 0);
}

void ilrd::WriteResponse::OnSuccess(ilrd::NBDProxy *nbd_proxy, Uid uid, std::shared_ptr<char[]> buffer, uint64_t len)
{
    (void) buffer; 
    (void) len; 
    nbd_proxy->NBDWriteResponse(uid, ilrd::enums::status::SUCCESS);
}

void ilrd::WriteResponse::OnFailure(ilrd::NBDProxy *nbd_proxy, Uid uid)
{
    nbd_proxy->NBDWriteResponse(uid, ilrd::enums::status::FAILURE);
}
