#include <iostream>

extern "C"
{
__attribute__((constructor))
static void Print()
{
    std::cout << "The dll is explicity load!" << std::endl;
}
}