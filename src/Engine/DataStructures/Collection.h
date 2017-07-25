#ifndef COLLECTION_H
#define COLLECTION_H

#include <iostream>
#include <algorithm>
#include <functional>

template <class Iterator>
class CollectionRange
{
public:
    CollectionRange(Iterator itBegin, Iterator itEnd) :
        m_beginEnd( std::make_pair(itBegin, itEnd) )
    {
    }

    Iterator Begin() const { return m_beginEnd.first; }
    Iterator End()   const { return m_beginEnd.second; }

private:
    std::pair<Iterator, Iterator> m_beginEnd;
};



class Collection
{
public:
    template <class Iterator, class T>
    static Iterator Find(const CollectionRange<Iterator> &col, const T &x)
    {
        return std::find(col.Begin(), col.End(), x);
    }
    template <class Iterator, class T>
    static bool Contains(const CollectionRange<Iterator> &col, const T &x)
    {
        return Collection::Find(col, x) != col.End();
    }

    template <class Iterator, class T>
    static uint Count(const CollectionRange<Iterator> &col, const T& x)
    {
        int count = 0;
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            if (x == *it) { ++count; }
        }
        return count;
    }

private:
    Collection()
    {
    }

    virtual ~Collection()
    {
    }
};

#endif // COLLECTION_H
