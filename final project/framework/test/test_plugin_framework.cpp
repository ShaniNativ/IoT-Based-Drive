#include <iostream>

#include "framework.hpp"
#include "factory.hpp"
#include "task_args.hpp"
#include "handleton.hpp"
#include "minion_manager.hpp"

using namespace ilrd;
class UpdatedReadCommand : public ICommand
{
public:
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args)
    {

        std::cout << "Run" << std::endl;
        std::shared_ptr<ilrd::ReadArgs> read_args = static_pointer_cast<ilrd::ReadArgs>(args); 
        ilrd::Handleton<ilrd::MinionManager>::GetInstace()->AddReadTask(read_args->GetOffset(), read_args->GetLength(), read_args->GetUID());
        std::cout << "Test Read to buffer:" << static_pointer_cast<ilrd::ReadArgs>(args)->GetLength() << " " << static_pointer_cast<ilrd::ReadArgs>(args)->GetOffset() << " "  << std::endl;          
   
        return std::make_pair(nullptr, static_cast<std::chrono::milliseconds>(0)); 
    }
};


std::shared_ptr<ICommand> CreateUpdatedReadCommand()
{
    std::cout << "CreateUpdatedReadCommand " << std::endl;
    return std::make_shared<UpdatedReadCommand>(UpdatedReadCommand());
}

extern "C"
{
__attribute__((constructor))
void SoMain()
{
    std::cout << "Test Dll Load - test" << std::endl;
    std::cout << ilrd::Handleton<ilrd::Factory<int, ilrd::ICommand>>::GetInstace() << std::endl;
    ilrd::Handleton<ilrd::Factory<int, ilrd::ICommand>>::GetInstace()->Register(0,CreateUpdatedReadCommand);  
} 

}