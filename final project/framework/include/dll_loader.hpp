/*** 
Written by: Shani Davidian
Reviewed by: Itai
Date: 18.3.25
Exercise: Dir Monitor
***/

#ifndef ILRD_RD1645_DLLLOADER_HPP
#define ILRD_RD1645_DLLLOADER_HPP

#include <string> /* std::string */
#include <unordered_map> /* std::unordered_map */

namespace ilrd
{
    class DLLloader
    {
    public:
        DLLloader() = default; 
        ~DLLloader(); 
        void Load(const std::string& path_name);
        void UnLoad(const std::string& path_name);
        void* GetDll(const std::string& path_name);
    
    private:
        typedef void*  dll_handler;  
        std::unordered_map<std::string, dll_handler> m_hanldes_map; 
    };
}//ilrd


#endif //ILRD_RD1645_DLLLOADER_HPP