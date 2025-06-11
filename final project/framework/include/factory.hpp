
/*** 
Written by: Shani Davidian
Reviewed by: Ofir
Date: 10.3.25
Exercise: Factory
***/
#ifndef ILRD_RD1645_FACTORY_HPP
#define ILRD_RD1645_FACTORY_HPP

#include <functional> /* std::function, std::tuple */
#include <unordered_map> /* std::unordered_map */
#include <memory> /* std::shared_ptr */
#include "handleton.hpp"

namespace ilrd
{

template <typename KEY, typename BASE, typename... ARGS>
class Factory
{
public:
    Factory(const Factory&) = delete; 
    Factory& operator=(const Factory&) = delete; 
    //Set the key,Creator in the map
    void Register(KEY key, std::function<std::shared_ptr<BASE>(ARGS...)> Creator);
    //Create the object, return BASE*
    std::shared_ptr<BASE> Creator(KEY key, ARGS... args); 
    friend class ilrd::Handleton<Factory>; //?? need to check this is right

private:
    Factory() = default;
    std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_factory_map; 
    
};

template<typename KEY, typename BASE, typename... ARGS> 
void ilrd::Factory<KEY, BASE, ARGS...>::Register(KEY key, std::function<std::shared_ptr<BASE>(ARGS...)> Creator) 
{
    m_factory_map[key] = Creator;
}

template<class KEY, class BASE, class... ARGS> 
std::shared_ptr<BASE> ilrd::Factory<KEY, BASE, ARGS...>::Creator(KEY key, ARGS... args)
{ 
   return m_factory_map.at(key)(args...);
}

}

#endif //ILRD_RD1645_FACTORY_HPP