/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 27.3.25
Exercise: Framework
***/

#include <iostream>
#include <list>
#include <cstring>
#include "framework.hpp"

using namespace ilrd; 


static std::atomic_int counter = true; 
enum key  {READ = 0, WRITE =1}; 

//gdpp23 -fPIC -shared -o libshared_dll.so -I./include  src/thread_pool.cpp src/scheduler.cpp src/logger.cpp  src/handleton.cpp
//gdpp23 test/test_framework.cpp src/framework.cpp src/listener.cpp src/reactor.cpp src/dir_monitor.cpp src/dll_loader.cpp src/async_injection.cpp -I./include -Wl,-rpath,. -L. -lshared_dll
//gdpp23 -fPIC -shared -I./include test/test_plugin_framework.cpp -o libdll.so
class NBDReadArgs : public ITaskArgs
{
public:

    NBDReadArgs(int offset = 0, int len = 1) : m_offset(offset), m_length(len)
    {

    }

    int GetKey()
    {
        return 0; //enum of READ
    }

    int GetOffset()
    {
        return m_offset; 
    }

    int GetLength()
    {
        return m_length;
    }

private:
    size_t m_offset; 
    size_t m_length; 

}; // offset, length (Getters / Setters / ctor)

class NBDWriteArgs : public ITaskArgs
{
public:

    NBDWriteArgs(int offset = 0, int len= 1) : m_offset(offset), m_length(len), m_buffer(new char[5])
    {

    }

    int GetKey()
    {
        return key::WRITE;
    }

    int GetOffset()
    {
        return m_offset; 
    }

    int GetLength()
    {
        return m_length;
    }

    void SetOffset(int offset)
    {
         m_offset = offset; 
    }

    void SetLength(int len)
    {
         m_length = len;
    }

    std::shared_ptr<char[]>  GetBuffer()
    {
        return m_buffer;
    }

    void  SetBuffer()
    {
        //std::memcpy(&m_buffer, "Hello", 5);
        m_buffer.reset(new char[]("Hello"));

    }

private:
    size_t m_offset; 
    size_t m_length; 
    std::shared_ptr<char[]> m_buffer; 

}; // offset, length,  char* (Getters / Setters / ctor)

class NBDProxy : public IInputProxy
{
    std::shared_ptr<ITaskArgs> GetTaskArgs(int fd, ilrd::IListener::mode mode)
    {

        if((++counter)%2 == 0)
        {
            //read from fd 
            char buf[BUFSIZ] = {0};
            read(fd, buf, BUFSIZ); // CLEAR THE FD 
            return  std::make_shared<NBDReadArgs>(NBDReadArgs()); //static bool to return once read_args once write_args
        }
        else
        {
            return std::make_shared<NBDReadArgs>(NBDWriteArgs()); 
        }
    }
}; // GetTaskArgs() which returns NBDReadArgs or NBDWriteArgs with some test values

class CommandRead : public ICommand
{/*...*/
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(ITaskArgs* args)
    {
        std::cout << "Test Read to buffer:" << dynamic_cast<NBDReadArgs*>(args)->GetLength() << " " <<
        dynamic_cast<NBDReadArgs*>(args)->GetOffset() << " "  << std::endl;          
        return std::make_pair(nullptr, static_cast<std::chrono::milliseconds>(0)); 
    }

}; // Execute() which prints the fields

class CommandWrite : public ICommand
{/*...*/
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(ITaskArgs* args)
    {
        std::cout << "Test Write to buffer:" << dynamic_cast<NBDWriteArgs*>(args)->GetLength() << " " <<
        dynamic_cast<NBDWriteArgs*>(args)->GetOffset() << " "  << std::endl;        
        return std::make_pair(nullptr, static_cast<std::chrono::milliseconds>(0)); 
    }

}; // Execute() which prints the fields

std::shared_ptr<ICommand> CreateReadCommand()
{
    return std::make_shared<CommandRead>(CommandRead());
};

std::shared_ptr<ICommand>  CreateWriteCommand()
{
    return std::make_shared<CommandWrite>(CommandWrite());
}

int main()
{
    NBDProxy nbd_proxy;

    Framework::FDModeCallback input_proxies = {STDIN_FILENO, ilrd::IListener::READ, &nbd_proxy}; 
    std::list<Framework::FDModeCallback> input_proxies_list = {input_proxies};
    std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>> creators = 
                                    {{key::READ, CreateReadCommand},{key::WRITE,CreateWriteCommand}};   
    Framework fw(input_proxies_list,"./plugins",creators);
    fw.Run();
    
    // Now enter something in the terminal
    
    // Note:
    // a. No need to parse std::in input
    // b. Take list by const reference
    // c. Test both Read/Write
    // d. Test plugins.

    return 0;
}
