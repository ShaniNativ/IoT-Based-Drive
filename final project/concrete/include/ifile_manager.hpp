/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 20.4.25
Exercise: Final Project - Minion part
***/

#ifndef ILRD_RD1645_IFILE_MANAGER_HPP
#define ILRD_RD1645_IFILE_MANAGER_HPP

#include <cstdint>
#include <memory>

#include "definitions.hpp"

namespace ilrd
{
class IFileManager 
{
public:
    virtual enums::status Read(uint64_t len , uint64_t offset, std::shared_ptr<char[]> buffer) = 0; 
    virtual enums::status Write(uint64_t len , uint64_t offset, std::shared_ptr<char[]> data_to_write) = 0; 
    virtual ~IFileManager() = default;

};
}

#endif //ILRD_RD1645_IFILE_MANAGER_HPP