/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 6.4.25
Exercise: Final Project - Master part
***/

#ifndef ILRD_RD1645_DEFINITIONS_HPP
#define ILRD_RD1645_DEFINITIONS_HPP

namespace ilrd::enums
{
enum mode
{
    READ = 0, 
    WRITE = 1
};

enum status
{
    SUCCESS = 0, 
    FAILURE = 1
};

enum message_type
{
    READ_REQUEST = 0, 
    WRITE_REQUEST = 1, 
    READ_RESPONSE = 2, 
    WRITE_RESPONSE = 3

};
}
#endif //ILRD_RD1645_DEFINITIONS_HPP