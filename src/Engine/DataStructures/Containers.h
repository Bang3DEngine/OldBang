#ifndef COLLECTION_H
#define COLLECTION_H

#include <iostream>
#include <algorithm>
#include <functional>

#include "Bang.h"

class Containers
{
public:
    template <template <class T> class Container, class T>
    static typename Container<T>::Iterator Find(Container<T> &cont,
                                                const T &x)
    {
        return std::find(cont.Begin(), cont.End(), x);
    }

    template <template <class T> class Container, class T>
    static typename Container<T>::Const_Iterator Find(const Container<T> &cont,
                                                      const T &x)
    {
        return std::find(cont.Begin(), cont.End(), x);
    }

    template <template <class T> class Container, class T>
    static bool Contains(const Container<T> &cont, const T &x)
    {
        return Containers::Find(cont, x) != cont.End();
    }

    template <template <class T> class Container, class T>
    static uint Count(const Container<T> &cont, const T& x)
    {
        int count = 0;
        for (typename Container<T>::Iterator it = cont.Begin();
             it != cont.End(); ++it)
        {
            if (x == *it) { ++count; }
        }
        return count;
    }

    Containers() = delete;
    virtual ~Containers() = delete;
};

#endif // COLLECTION_H
