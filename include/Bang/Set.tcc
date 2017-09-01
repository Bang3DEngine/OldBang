#ifndef SET_TCC
#define SET_TCC

#include "Bang/Set.h"

template<class Key>
Set<Key>::Set()
{
}

template<class Key>
Set<Key>::Set(const std::set<Key> &s) : m_set(s)
{
}

template<class Key>
void Set<Key>::Add(const Key &key)
{
    m_set.insert(key);
}

template<class Key>
template<class OtherIterator>
void Set<Key>::Add(OtherIterator itBegin, OtherIterator itEnd)
{
    m_set.insert(itBegin, itEnd);
}

template<class Key>
void Set<Key>::Remove(const Key &key)
{
    m_set.erase(key);
}

template<class Key>
typename Set<Key>::Iterator Set<Key>::Remove(typename Set<Key>::Iterator it)
{
    return m_set.erase(it);
}

template<class Key>
void Set<Key>::Clear()
{
    m_set.clear();
}

template<class Key>
int Set<Key>::Size() const
{
    return m_set.size();
}

template<class Key>
bool Set<Key>::IsEmpty() const
{
    return m_set.empty();
}

template<class Key>
bool Set<Key>::Contains(const Key &key) const
{
    return m_set.find(key) != m_set.end();
}

template<class Key>
List<Key> Set<Key>::GetKeys() const
{
    List<Key> result;
    for (Key &k : *this) { result.PushBack(k); }
    return result;
}

template<class Key>
typename Set<Key>::Iterator
Set<Key>::Begin() { return m_set.begin(); }

template<class Key>
typename Set<Key>::Iterator
Set<Key>::End() { return m_set.end(); }

template<class Key>
typename Set<Key>::Const_Iterator
Set<Key>::Begin() const { return m_set.cbegin(); }

template<class Key>
typename Set<Key>::Const_Iterator
Set<Key>::End() const { return m_set.cend(); }

template<class Key>
typename Set<Key>::RIterator
Set<Key>::RBegin() { return m_set.rbegin(); }

template<class Key>
typename Set<Key>::RIterator
Set<Key>::REnd() { return m_set.rend(); }

template<class Key>
typename Set<Key>::Const_RIterator
Set<Key>::RBegin() const { return m_set.crbegin(); }

template<class Key>
typename Set<Key>::Const_RIterator
Set<Key>::REnd() const { return m_set.crend(); }

template<class Key>
typename Set<Key>::Iterator
Set<Key>::begin() { return m_set.begin(); }

template<class Key>
typename Set<Key>::Iterator
Set<Key>::end() { return m_set.end(); }

template<class Key>
typename Set<Key>::Const_Iterator
Set<Key>::cbegin() const { return m_set.cbegin(); }

template<class Key>
typename Set<Key>::Const_Iterator
Set<Key>::cend() const { return m_set.cend(); }

#endif // SET_TCC
