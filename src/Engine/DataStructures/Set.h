#ifndef SET_H
#define SET_H

#include <set>
#include "Bang.h"

FORWARD_T class List;

template<class Key>
class Set
{
public:
    using Iterator = typename std::set<Key>::iterator;
    using RIterator = typename std::set<Key>::reverse_iterator;
    using Const_Iterator = typename std::set<Key>::const_iterator;
    using Const_RIterator = typename std::set<Key>::const_reverse_iterator;

    Set()
    {
    }

    Set(const std::set<Key> &s) : m_set(s)
    {
    }

    void Insert(const Key &key)
    {
        m_set.insert(key);
    }

    void Remove(const Key &key)
    {
        m_set.erase(key);
    }

    Iterator Remove(Iterator it)
    {
        return m_set.erase(it);
    }

    void Clear()
    {
        m_set.clear();
    }

    int Size() const
    {
        return m_set.size();
    }

    bool Empty() const
    {
        return m_set.empty();
    }

    bool Contains(const Key &key) const
    {
        return m_set.find(key) != m_set.end();
    }

    List<Key> GetKeys() const
    {
        List<Key> result;
        for (Key &k : *this) { result.PushBack(k); }
        return result;
    }

    Iterator Begin() { return m_set.begin(); }
    Iterator End() { return m_set.end(); }
    Const_Iterator Begin() const { return m_set.cbegin(); }
    Const_Iterator End() const { return m_set.cend(); }
    RIterator RBegin() { return m_set.rbegin(); }
    RIterator REnd() { return m_set.rend(); }
    Const_RIterator RBegin() const { return m_set.crbegin(); }
    Const_RIterator REnd() const { return m_set.crend(); }

    // To allow range-based for loops
    Iterator begin() { return m_set.begin(); }
    Iterator end() { return m_set.end(); }
    Const_Iterator cbegin() const { return m_set.cbegin(); }
    Const_Iterator cend() const { return m_set.cend(); }

private:
    std::set<Key> m_set;
};

#endif // SET_H
