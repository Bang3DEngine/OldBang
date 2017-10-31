#ifndef SET_H
#define SET_H

#include <set>
#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD_T class List;

template<class Key>
class Set
{
public:
    using Iterator = typename std::set<Key>::iterator;
    using RIterator = typename std::set<Key>::reverse_iterator;
    using Const_Iterator = typename std::set<Key>::const_iterator;
    using Const_RIterator = typename std::set<Key>::const_reverse_iterator;

    Set();
    Set(const std::set<Key> &s);

    void Add(const Key &key);

    template<class OtherIterator>
    void Add(OtherIterator itBegin, OtherIterator itEnd);

    void Remove(const Key &key);
    Iterator Remove(Iterator it);

    void Clear();
    int Size() const;
    bool IsEmpty() const;
    bool Contains(const Key &key) const;

    List<Key> GetKeys() const;

    Iterator Begin();
    Iterator End();
    Const_Iterator Begin() const;
    Const_Iterator End() const;
    RIterator RBegin();
    RIterator REnd();
    Const_RIterator RBegin() const;
    Const_RIterator REnd() const;

    // To allow range-based for loops
    Iterator begin();
    Iterator end();
    Const_Iterator cbegin() const;
    Const_Iterator cend() const;

private:
    std::set<Key> m_set;
};

NAMESPACE_BANG_END

#include "Set.tcc"

#endif // SET_H
