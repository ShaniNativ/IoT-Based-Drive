

#include "message.hpp"      
#include "handleton.hpp"
#include "factory.hpp"
#include "definitions.hpp"
#include "uid.hpp"

#include <iostream>
#include <cstring>

using namespace ilrd;
using namespace std;


static std::shared_ptr<AMessage> ReadRequestCreate()
{
    return std::make_shared<MessageReadRequest>(MessageReadRequest());
}

static std::shared_ptr<AMessage> WriteRequestCreate()
{
    return std::make_shared<MessageWriteRequest>(MessageWriteRequest());
}

static std::shared_ptr<AMessage> ReadResponseCreate()
{
    return std::make_shared<MessageReadResponse>(MessageReadResponse());
}

static std::shared_ptr<AMessage> WriteResponseCreate()
{
    return std::make_shared<MessageWriteResponse>(MessageWriteResponse());
}

static void InitFactory()
{
    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>
              ::GetInstace()->Register(enums::message_type::READ_REQUEST, ReadRequestCreate);

    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>
              ::GetInstace()->Register(enums::message_type::WRITE_REQUEST, WriteRequestCreate);

    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>
              ::GetInstace()->Register(enums::message_type::READ_RESPONSE, ReadResponseCreate);

    ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>
        ::GetInstace()->Register(enums::message_type::WRITE_RESPONSE, WriteResponseCreate);
}

static shared_ptr<AMessage> Recieve(char* buffer)
{
    uint64_t size = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);
    ilrd::enums::message_type class_type = *(ilrd::enums::message_type*)buffer;
    buffer += sizeof(ilrd::enums::message_type);

    shared_ptr<AMessage> obj = Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>
                ::GetInstace()->Creator(class_type); 
    
    obj->FromBuffer(buffer); 

    return obj;

}

static void NewReadRequest()
{
    Uid uid;
    shared_ptr<MessageReadRequest>  msg(new MessageReadRequest(uid, 0, 10));
    char* buffer = new char[msg->GetSize()];
    msg->ToBuffer(buffer); 
    Recieve(buffer); 
    delete[] buffer; 

}

static void NewWriteRequest()
{
    Uid uid;
    std::shared_ptr<char[]> str(new char[10]); 
    memcpy(str.get(), "1234567890", 10); 
    shared_ptr<MessageWriteRequest>  msg(new MessageWriteRequest(uid, 0, 10, str));
    char* buffer = new char[msg->GetSize()];
    msg->ToBuffer(buffer); 
    Recieve(buffer); 
    delete[] buffer; 

}

static void NewReadResponse()
{
  Uid uid;
  std::shared_ptr<char[]> str(new char[11]); 
  memcpy(str.get(), "1234567890", 10); 
  shared_ptr<MessageReadResponse>  msg(new MessageReadResponse(enums::status::SUCCESS, 10, str));

  size_t size = msg->GetSize(); 
  std::cout << msg->GetSize() << std::endl; 
  char* buffer = new char[size];
  msg->ToBuffer(buffer); 
  Recieve(buffer); 
  delete[] buffer; 
}

static void NewWriteResponse()
{
  Uid uid;

  shared_ptr<MessageWriteResponse>  msg(new MessageWriteResponse(enums::status::SUCCESS));
  char* buffer = new char[msg->GetSize()];
  msg->ToBuffer(buffer); 
  Recieve(buffer); 
  delete[] buffer; 
}


int main()
{
    InitFactory();


    NewReadRequest();
    NewWriteRequest();
    NewReadResponse();
    NewWriteResponse(); 


    return 0;
}
