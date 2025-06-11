/*** 
Written by: Shani Davidian
Reviewed by: Amit
Date: 18.3.25
Exercise: Dispatcher
***/

#include <string>
#include <iostream>
#include "dispatcher.hpp"

class Person
{
public:
    Person() = default; 
    void Print(std::string msg)
    {
        std::cout << this << " Person object: "<< msg << std::endl;
    }

};

class Teacher
{
public:
Teacher() = default; 
    void Print(std::string msg)
    {
        std::cout << this << " Teacher object: "<< msg << std::endl;
    }

    void Death()
    {
        std::cout << this << " Teacher object: Dispatche Death" << std::endl;
    }

    void PrintInt(int msg)
    {
        std::cout << this << " Teacher object: "<< msg << std::endl;
    }

};

void Test1()
{
    ilrd::Dispatcher<std::string> event_dispatcher; 
  //  ilrd::Dispatcher<int> event_dispatcher2; 
    Person p1; 
    Person p2; 
    Teacher p3; 
    Teacher p4; 


    ilrd::Callback<Person, std::string> callback_p1(p1, &Person::Print );
    ilrd::Callback<Person, std::string> callback_p2(p2, &Person::Print );
    ilrd::Callback<Teacher, std::string> callback_p3(p3, &Teacher::Print );
   // ilrd::Callback<Teacher, int> callback_p4(p4, &Teacher::PrintInt );

    //callback_p4.SetDispatcher(&event_dispatcher2); 


    event_dispatcher.Register(&callback_p1);
    event_dispatcher.Register(&callback_p2);
    event_dispatcher.Register(&callback_p3);
    std::string str = "This is a  message!";
    std::string str2 = "This is a second message!";
    event_dispatcher.Notify(str);

    std::cout << "Unregister Person object" << std::endl; 
    event_dispatcher.Unregister(&callback_p2);
    event_dispatcher.Notify(str);

    {
        Teacher p4; 
        ilrd::Callback<Teacher, std::string> callback_p4(p4, &Teacher::Print );

        event_dispatcher.Register(&callback_p4);
        std::cout << "Register Teacher local scope object" << std::endl; 
        event_dispatcher.Notify(str2);

    }

    std::cout << "Dtor Unregister Teacher object" << std::endl; 
   
    event_dispatcher.Notify(str2);

}

void Test2()
{
    Person p1; 
    Person p2; 
    Teacher p3; 
    ilrd::Callback<Person, std::string> callback_p1(p1, &Person::Print );
    ilrd::Callback<Person, std::string> callback_p2(p2, &Person::Print );
    ilrd::Callback<Teacher, std::string> callback_p3(p3, &Teacher::Print, &Teacher::Death);

    {
        ilrd::Dispatcher<std::string> event_dispatcher; 

        event_dispatcher.Register(&callback_p1);
        event_dispatcher.Register(&callback_p2);
        event_dispatcher.Register(&callback_p3);
        std::string str = "Dispatcher in local scope!";
        event_dispatcher.Notify(str);
    }
}

int main()
{
    Test1(); 
    Test2(); 

    return 0; 
}