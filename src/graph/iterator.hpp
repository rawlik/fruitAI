#ifndef _ITERATOR_HPP
#define _ITERATOR_HPP

template <typename T>
class Iterator
{
public:
    virtual ~Iterator();
    virtual bool IsDone() = 0;
    virtual T & operator*() = 0;
    virtual void operator++() = 0;
};

template <typename T>
Iterator<T>::~Iterator()
{ }

#endif
