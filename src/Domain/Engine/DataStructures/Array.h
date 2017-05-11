#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

#include "Bang/Collection.h"

template <class T>
class Array : public Collection< std::vector, T >
{
private: typedef class Collection< std::vector, T > Coll;
public:
    Array() { }
    Array(const std::vector<T> &v)      : Coll(v) { }
    Array(int size)                     : Coll(size) { }
    Array(int size, const T& initValue) : Coll(size, initValue) { }
    Array(std::initializer_list<T> l)   : Coll(l) { }

    void PushBack(const T& x) { this->push_back(x); }
    void Add(const T &x) override { this->PushBack(x); }
    const T& Front() const { return this->front(); }
    const T& Back() const  { return this->back(); }
    T& Front() { return this->front(); }
    T& Back()  { return this->back(); }
    T& PopBack()
    {
        T& x = Back();
        this->pop_back();
        return x;
    }

protected:
    char GetToStringOpeningBrace() const override { return '['; }
    char GetToStringClosingBrace() const override { return ']'; }
};

#endif // ARRAY_H
