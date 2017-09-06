#ifndef COLLECTION_H
#define COLLECTION_H

#include <iostream>
#include <algorithm>
#include <functional>

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Containers
{
public:
    template <template <class T> class Container, class T>
    static typename Container<T>::Iterator Find(Container<T> &cont,
                                                const T &x)
    {
        return ::std::find(cont.Begin(), cont.End(), x);
    }

    template <template <class T> class Container, class T>
    static typename Container<T>::Const_Iterator Find(const Container<T> &cont,
                                                      const T &x)
    {
        return ::std::find(cont.Begin(), cont.End(), x);
    }

    template <template <class T> class Container, class T>
    static bool Contains(const Container<T> &cont, const T &x)
    {
        return Containers::Find(cont, x) != cont.End();
    }

    template <class Iterator, class T>
    static uint Count(Iterator begin, Iterator end, const T& x)
    {
        int count = 0;
        for (Iterator it = begin; it != end; ++it)
        {
            if (x == *it) { ++count; }
        }
        return count;
    }

    template <template <class T> class Container, class T>
    static uint Count(const Container<T> &cont, const T& x)
    {
        return Containers::Count(cont.Begin(), cont.End(), x);
    }

    Containers() = delete;
};

NAMESPACE_BANG_END

#endif // COLLECTION_H
