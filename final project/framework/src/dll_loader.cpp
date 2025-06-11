/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Dir Monitor
***/

#include <stdexcept> /* std::runtime_error */
#include <dlfcn.h> /* dlopen */

#include "iostream" /* test only */
#include "dll_loader.hpp"

/*=========================== DLLLoad IMPLEMENTATION =========================*/

void ilrd::DLLloader::Load(const std::string& path_name)
{
    dll_handler handle = dlopen(path_name.c_str(),RTLD_NOW); /* ? error */
    std::cout << path_name.c_str() << std::endl; 
    if(NULL == handle)
    {
        throw std::runtime_error("Failed load dll");
    }

    m_hanldes_map[path_name] = handle; 
}

ilrd::DLLloader::~DLLloader()
{
    for(auto it = m_hanldes_map.begin(); it != m_hanldes_map.end(); ++it)
    {
        dlclose(it->second); 
    }
}

void ilrd::DLLloader::UnLoad(const std::string&path_name)
{
    if(m_hanldes_map.find(path_name) == m_hanldes_map.end())
    {   
        throw std::runtime_error("Handle dll does not exist");
    }
    
    dlclose(m_hanldes_map[path_name]); 
    m_hanldes_map.erase(path_name); 
}

void* ilrd::DLLloader::GetDll(const std::string& path_name)
{
    return m_hanldes_map[path_name]; 
}
