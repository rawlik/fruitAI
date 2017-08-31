#ifndef _VISITOR_HPP
#define _VISITOR_HPP

template <typename T>
class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(T & element) = 0;
    virtual bool IsDone();
};

template <typename T>
Visitor<T>::~Visitor()
{ }

template <typename T>
bool Visitor<T>::IsDone()
{
    return false;
}

#endif
