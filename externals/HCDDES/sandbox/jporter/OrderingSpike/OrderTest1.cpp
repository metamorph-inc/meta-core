#include "OrderTest1.h"

OrderTest1::OrderTest1()
{
    //ctor

}

OrderTest1::~OrderTest1()
{
    //dtor
}

OrderTest1::OrderTest1(const OrderTest1& other)
{
    //copy ctor
}

OrderTest1& OrderTest1::operator=(const OrderTest1& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
