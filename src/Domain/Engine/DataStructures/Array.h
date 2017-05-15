#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

#include "Bang/Container.h"

template <class T>
class Array : public Container< std::vector, T >
{
    CONTAINER(std::vector)
public:
    Array() { }
    Array(const std::vector<T> &v)      : Container(v) { }
    Array(int size)                     : Container(size) { }
    Array(int size, const T& initValue) : Container(size, initValue) { }
    Array(std::initializer_list<T> l)   : Container(l) { }
    virtual ~Array() {}

protected:
    char GetToStringOpeningBrace() const override { return '['; }
    char GetToStringClosingBrace() const override { return ']'; }
};

#endif // ARRAY_H
