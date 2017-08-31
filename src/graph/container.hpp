#ifndef _CONTAINER_HPP
#define _CONTAINER_HPP

#include "iterator.hpp"
#include "visitor.hpp"

template <typename T>
class Container
{
public:
    virtual ~Container();

    virtual int Count() = 0;
    virtual bool IsEmpty();
    virtual bool IsFull();
    virtual void MakeNull() = 0;

    virtual void Accept(Visitor<T> & visitor) = 0;
    virtual Iterator<T> & NewIterator() = 0;
};

template <typename T>
Container<T>::~Container()
{ }

template <typename T> 
bool Container<T>::IsEmpty()
{
    return Count() == 0;
}

template <typename T>
bool Container<T>::IsFull()
{
    return false;
}

#endif
