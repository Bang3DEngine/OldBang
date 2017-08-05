#ifndef MAP_H
#define MAP_H

#include <map>
#include "Bang.h"

FORWARD_T class List;

template <class Key, class Value>
class Map
{
public:
    using Iterator = typename std::map<Key, Value>::iterator;
    using RIterator = typename std::map<Key, Value>::reverse_iterator;
    using Const_Iterator = typename std::map<Key, Value>::const_iterator;
    using Const_RIterator = typename std::map<Key, Value>::const_reverse_iterator;

    Map() {}
    Map(const std::map<Key, Value> &m) : m_map(m) {}

    void Set(const Key &key, const Value &value)
    {
        (*this)[key] = value;
    }

    void Remove(const Key &key)
    {
        m_map.erase(key);
    }

    Iterator Remove(Iterator it)
    {
        return m_map.erase(it);
    }

    void RemoveValues(const Value &value)
    {
        for (auto it = Begin(); it != End(); )
        {
            if (it->second == value) { it = Remove(it); }
            else { ++it; }
        }
    }

    Value& Get(const Key &key)
    {
        return (*this)[key];
    }

    const Value& Get(const Key &key) const
    {
        return m_map.at(key);
    }

    void Clear()
    {
        m_map.clear();
    }

    int Size() const
    {
        return m_map.size();
    }

    bool Empty() const
    {
        return Size() == 0;
    }

    bool ContainsKey(const Key &key) const
    {
        return Find(key) != End();
    }

    bool ContainsValue(const Value &value) const
    {
        for (auto it = Begin(); it != End(); ++it)
        {
            if (it->second == value) return true;
        }
        return false;
    }

    Iterator Find(const Key &key)
    {
        return m_map.find(key);
    }
    Const_Iterator Find(const Key &key) const
    {
        return m_map.find(key);
    }

    List<Key> GetKeysWithValue(const Value& v) const
    {
        List<Key> result;
        for (auto it = Begin(); it != End(); ++it)
        {
            if (it->second == v) { result.PushBack(it->first); }
        }
        return result;
    }

    List<Key> GetKeys() const
    {
        List<Key> result;
        for (const auto& it : *this) { result.PushBack(it.first); }
        return result;
    }

    List<Value> GetValues() const
    {
        List<Value> result;
        for (auto it = Begin(); it != End(); ++it)
        {
            result.PushBack(it->second);
        }
        return result;
    }

    Value& At(const Key &k) { return m_map.at(k); }
    const Value& At(const Key &k) const { return m_map.at(k); }
    Value& operator[](const Key &k) { return m_map[k]; }
    const Value& operator[](const Key &k) const { return m_map[k]; }

    Iterator Begin() { return m_map.begin(); }
    Iterator End() { return m_map.end(); }
    Const_Iterator Begin() const { return m_map.cbegin(); }
    Const_Iterator End() const { return m_map.cend(); }
    RIterator RBegin() { return m_map.rbegin(); }
    RIterator REnd() { return m_map.rend(); }
    Const_RIterator RBegin() const { return m_map.crbegin(); }
    Const_RIterator REnd() const { return m_map.crend(); }

    // To allow range-based for loops
    Iterator begin() { return m_map.begin(); }
    Iterator end() { return m_map.end(); }
    Const_Iterator cbegin() const { return m_map.cbegin(); }
    Const_Iterator cend() const { return m_map.cend(); }

private:
    std::map<Key, Value> m_map;
};
#endif // MAP_H
