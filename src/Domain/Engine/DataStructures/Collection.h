#ifndef COLLECTION_H
#define COLLECTION_H

#include <iostream>
#include <algorithm>
#include <functional>

#include "Bang/IToString.h"

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
    static bool Any(const CollectionRange<Iterator> &col,
                    std::function< bool(const T&) > boolPredicate)
    {
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            if (boolPredicate(*it)) { return true; }
        }
        return false;
    }

    template <class Iterator, class T>
    static bool All(const CollectionRange<Iterator> &col,
                    std::function< bool(const T&) > boolPredicate)
    {
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            if (!boolPredicate(*it)) { return false; }
        }
        return true;
    }

    template <class Iterator, class T>
    static bool None(const CollectionRange<Iterator> &col,
                     std::function< bool(const T&) > boolPredicate)
    {
        return !Collection::Any(col, boolPredicate);
    }

    template <class Iterator, class T>
    static uint Count(const CollectionRange<Iterator> &col,
                      std::function< bool(const T&) > boolPredicate)
    {
        uint count = 0;
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            if (boolPredicate(*it)) { ++count; }
        }
        return count;
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

    template<class Iterator>
    static String ToString(const CollectionRange<Iterator> &col,
                           const String &openingTag,
                           const String &closingTag)
    {
        std::ostringstream oss;
        oss << openingTag;

        bool first = true;
        for (Iterator it = col.Begin(); it != col.End(); ++it)
        {
            if (!first) { oss << ", "; } else { first = false; }
            oss << &(*it);
        }
        oss << closingTag;

        return String(oss.str());
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
