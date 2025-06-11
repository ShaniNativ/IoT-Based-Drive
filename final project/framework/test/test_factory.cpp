
/*** 
Written by: Shani Davidian
Reviewed by: 
Date: 10.3.25
Exercise: Factory
***/
#include <iostream>
#include <factory.hpp>

class IShape
{
public:
    IShape() {};
    virtual ~IShape() = 0 ; 
}; 

IShape::~IShape() {}; 

class Rectangle : public IShape
{
public:
    Rectangle(int a = 10, int b = 10): m_a(a), m_b(b)
    {
        std::cout << m_a << "," << m_b << std::endl; 
    }

private:
    int m_a ; 
    int m_b; 
}; 

class Triangle : public IShape
{
public:
    Triangle(int a = 15, int b = 15): high(a), IShape(b)
    {
        std::cout << high << "," << IShape << std::endl; 
    }

private:
    int high; 
    int IShape; 
}; 

std::shared_ptr<IShape> CreateRectangle(int num1 = 10, int num2 = 10)
{
    std::cout << "Create Rectangle" << std::endl; 
    return std::make_shared<Rectangle>(Rectangle(num1, num2)); 
};



std::shared_ptr<IShape> CreateTriangle(int num1 = 15, int num2 = 15)
{
    std::cout << "Create Triangle" << std::endl; 
    return std::make_shared<Triangle>(Triangle(num1, num2));  
};

std::shared_ptr<IShape> CreateTriangle2()
{
    std::cout << "Create Triangle" << std::endl; 
    return std::make_shared<Triangle>(Triangle());  
};

std::shared_ptr<IShape> CreateRectangle2()
{
    std::cout << "Create Rectangle" << std::endl; 
    return std::make_shared<Rectangle>(Rectangle()); 
};

int main()
{

    ilrd::Factory<int, IShape, int, int> shape_factory;
    shape_factory.Register(1, CreateRectangle); 
    shape_factory.Register(2, CreateTriangle); 
    shape_factory.Creator(1, 5, 5);
    shape_factory.Creator(2, 5, 7);

    ilrd::Factory<int, IShape> shape_factory2;
    shape_factory2.Register(1, CreateRectangle2); 
    shape_factory2.Register(2, CreateTriangle2); 
    shape_factory2.Creator(1);
    shape_factory2.Creator(2);
    //shape_factory2.Creator(3);

    return 0; 
}