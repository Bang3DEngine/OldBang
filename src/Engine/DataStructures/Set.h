#ifndef SET_H
#define SET_H

#include <set>
#include "Bang.h"

FORWARD_T class List;

template<class Key>
class Set : private std::set<Key>
{
public:
    using Iterator = typename std::set<Key>::iterator;
    using RIterator = typename std::set<Key>::reverse_iterator;
    using Const_Iterator = typename std::set<Key>::const_iterator;
    using Const_RIterator = typename std::set<Key>::const_reverse_iterator;

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
    RIterator RBegin() { return this->rbegin(); }
    RIterator REnd() { return this->rend(); }
    Const_RIterator RBegin() const { return this->rbegin(); }
    Const_RIterator REnd() const { return this->rend(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::set<Key>::begin(); }
    Iterator end() { return this->std::set<Key>::end(); }
    Const_Iterator begin() const { return this->std::set<Key>::begin(); }
    Const_Iterator end() const { return this->std::set<Key>::end(); }
};

#endif // SET_H
