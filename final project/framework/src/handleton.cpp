/*** 
Written by: Shani Davidian
Reviewed by: Gil
Date: 17.3.25
Exercise: Handleton
***/

#define I_AM_THE_REAL_HANDLETON_IMP //handleton implementation

#include <handleton.hpp>
#include <thread_pool.hpp>
#include <scheduler.hpp>
#include <factory.hpp>
#include <framework.hpp>
#include <logger.hpp>

#include "minion_manager.hpp"
#include "response_manager.hpp"
#include "task_args.hpp"
#include "definitions.hpp"
#include "response.hpp"
#include "master_proxy.hpp"
#include "file_manager.hpp"

#include "config_file.hpp"

template class ilrd::Handleton<ilrd::ThreadPool>; 
template class ilrd::Handleton<ilrd::Scheduler>; 
template class ilrd::Handleton<ilrd::Factory< int, ilrd::ICommand>>; 
template class ilrd::Handleton<ilrd::Logger>; 

template class ilrd::Handleton<ilrd::MinionManager>; 
template class ilrd::Handleton<ilrd::ResponseManager>; 
template class ilrd::Handleton<ilrd::Factory<ilrd::enums::mode, ilrd::ATaskArgs>>; 
template class ilrd::Handleton<ilrd::Factory<ilrd::enums::mode, ilrd::IResponse>>; 
template class ilrd::Handleton<ilrd::Factory<ilrd::enums::message_type, ilrd::AMessage>>; 

template class ilrd::Handleton<ilrd::MasterProxy>; 
template class ilrd::Handleton<ilrd::FileManager>; 

template class ilrd::Handleton<ilrd::ConfigFile>;

