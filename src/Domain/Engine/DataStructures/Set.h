#ifndef SET_H
#define SET_H

#include <set>

#include "List.h"
#include "String.h"

template<class Key>
class Set : private std::set<Key>
{
public:
    typedef typename std::set<Key>::iterator Iterator;
    typedef typename std::set<Key>::const_iterator Const_Iterator;
    typedef typename std::set<Key>::reverse_iterator Reverse_Iterator;
    typedef typename std::set<Key>::const_reverse_iterator Const_Reverse_Iterator;

    Set()
    {
    }

    Set(const std::set<Key> &s) : std::set<Key>(s)
    {
    }

    void Insert(const Key &key)
    {
        this->insert(key);
    }

    void Remove(const Key &key)
    {
        this->erase(key);
    }

    Iterator Remove(Iterator it)
    {
        return this->erase(it);
    }

    void Clear()
    {
        this->clear();
    }

    int Size() const
    {
        return this->size();
    }

    bool Empty() const
    {
        return this->empty();
    }

    bool Contains(const Key &key) const
    {
        return this->find(key) != this->end();
    }

    List<Key> GetKeys() const
    {
        List<Key> result;
        for (Key &k : *this) { result.PushBack(k); }
        return result;
    }

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end(); }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }
    Reverse_Iterator RBegin() { return this->rbegin(); }
    Reverse_Iterator REnd() { return this->rend(); }
    Const_Reverse_Iterator RBegin() const { return this->rbegin(); }
    Const_Reverse_Iterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::set<Key>::begin(); }
    Iterator end() { return this->std::set<Key>::end(); }
    Const_Iterator begin() const { return this->std::set<Key>::begin(); }
    Const_Iterator end() const { return this->std::set<Key>::end(); }

    String ToString()
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (Key &k : *this)
        {
            if (!first) { oss << ", "; }
            oss << k;
            first = false;
        }
        oss << "}";
        return oss.str();
    }
};

#endif // SET_H
