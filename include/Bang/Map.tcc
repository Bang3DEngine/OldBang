#ifndef MAP_TCC
#define MAP_TCC

#include "Bang/Map.h"


NAMESPACE_BANG_BEGIN

template<class Key, class Value>
Map<Key, Value>::Map() {}

template<class Key, class Value>
Map<Key,Value>::Map(const std::map<Key, Value> &m) : m_map(m) {}

template<class Key, class Value>
void Map<Key,Value>::Add(const Key &key, const Value &value)
{
    (*this)[key] = value;
}

template<class Key, class Value>
void Map<Key,Value>::Remove(const Key &key)
{
    m_map.erase(key);
}

template<class Key, class Value>
typename Map<Key,Value>::Iterator
Map<Key,Value>::Remove(typename Map<Key,Value>::Iterator it)
{
    return m_map.erase(it);
}

template<class Key, class Value>
void Map<Key,Value>::RemoveValues(const Value &value)
{
    for (auto it = Begin(); it != End(); )
    {
        if (it->second == value) { it = Remove(it); }
        else { ++it; }
    }
}

template<class Key, class Value>
Value &Map<Key,Value>::Get(const Key &key)
{
    return (*this)[key];
}

template<class Key, class Value>
const Value &Map<Key,Value>::Get(const Key &key) const
{
    return m_map.at(key);
}

template<class Key, class Value>
void Map<Key,Value>::Clear()
{
    m_map.clear();
}

template<class Key, class Value>
int Map<Key,Value>::Size() const
{
    return m_map.size();
}

template<class Key, class Value>
bool Map<Key,Value>::IsEmpty() const
{
    return Size() == 0;
}

template<class Key, class Value>
bool Map<Key,Value>::ContainsKey(const Key &key) const
{
    return Find(key) != CEnd();
}

template<class Key, class Value>
bool Map<Key,Value>::ContainsValue(const Value &value) const
{
    for (auto it = Begin(); it != End(); ++it)
    {
        if (it->second == value) return true;
    }
    return false;
}

template<class Key, class Value>
typename Map<Key,Value>::Iterator Map<Key,Value>::Find(const Key &key)
{
    return m_map.find(key);
}

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::Find(const Key &key) const
{
    return m_map.find(key);
}

template<class Key, class Value>
List<Key> Map<Key,Value>::GetKeysWithValue(const Value &v) const
{
    List<Key> result;
    for (auto it = Begin(); it != End(); ++it)
    {
        if (it->second == v) { result.PushBack(it->first); }
    }
    return result;
}

template<class Key, class Value>
List<Key> Map<Key,Value>::GetKeys() const
{
    List<Key> result;
    for (const auto& it : *this) { result.PushBack(it.first); }
    return result;
}

template<class Key, class Value>
List<Value> Map<Key,Value>::GetValues() const
{
    List<Value> result;
    for (auto it = Begin(); it != End(); ++it)
    {
        result.PushBack(it->second);
    }
    return result;
}

template<class Key, class Value>
Value &Map<Key,Value>::At(const Key &k) { return m_map.at(k); }

template<class Key, class Value>
const Value &Map<Key,Value>::At(const Key &k) const { return m_map.at(k); }

template<class Key, class Value>
const Value &Map<Key,Value>::operator[](const Key &k) const { return m_map[k]; }

template<class Key, class Value>
typename Map<Key,Value>::Iterator
Map<Key,Value>::Begin() { return m_map.begin(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::Begin() const { return CBegin(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::CBegin() const { return m_map.cbegin(); }

template<class Key, class Value>
typename Map<Key,Value>::Iterator
Map<Key,Value>::End() { return m_map.end(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::End() const { return CEnd(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::CEnd() const { return m_map.cend(); }

template<class Key, class Value>
typename Map<Key,Value>::RIterator
Map<Key,Value>::RBegin() { return m_map.rbegin(); }

template<class Key, class Value>
typename Map<Key,Value>::RIterator
Map<Key,Value>::REnd() { return m_map.rend(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_RIterator
Map<Key,Value>::CRBegin() const { return m_map.crbegin(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_RIterator
Map<Key,Value>::CREnd() const { return m_map.crend(); }

template<class Key, class Value>
typename Map<Key,Value>::Iterator
Map<Key,Value>::begin() { return m_map.begin(); }

template<class Key, class Value>
typename Map<Key,Value>::Iterator
Map<Key,Value>::end() { return m_map.end(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::cbegin() const { return m_map.cbegin(); }

template<class Key, class Value>
typename Map<Key,Value>::Const_Iterator
Map<Key,Value>::cend() const { return m_map.cend(); }

template<class Key, class Value>
Value &Map<Key,Value>::operator[](const Key &k) { return m_map[k]; }

NAMESPACE_BANG_END

#endif // MAP_TCC

