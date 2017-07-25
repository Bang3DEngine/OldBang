#ifndef UMAP_H
#define UMAP_H

#include <sstream>
#include <iostream>
#include <functional>
#include <unordered_map>

template<class T> class List;

template <class Key, class Value>
class UMap : public std::unordered_map<Key, Value>
{
typedef std::unordered_map<Key, Value> BaseMap;

public:
    typedef typename BaseMap::iterator Iterator;
    typedef typename BaseMap::const_iterator Const_Iterator;

    UMap()
    {
    }

    UMap(const BaseMap &m) : BaseMap(m)
    {
    }

    void Set(const Key &key, const Value &value)
    {
        (*this)[key] = value;
    }

    void Remove(const Key &key)
    {
        this->erase(key);
    }

    Iterator Remove(Iterator it)
    {
        return this->erase(it);
    }

    void RemoveValues(const Value &value)
    {
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (it->second == value)
            {
                it = Remove(it);
                --it;
            }
        }
    }

    Value& Get(const Key &key)
    {
        return (*this)[key];
    }

    const Value& Get(const Key &key) const
    {
        return this->at(key);
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
        return Size() == 0;
    }

    bool ContainsKey(const Key &key) const
    {
        return this->Find(key) != this->End();
    }

    bool ContainsValue(const Value &value) const
    {
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (it->second == value) return true;
        }
        return false;
    }

    Iterator Find(const Key &key)
    {
        return this->find(key);
    }
    Const_Iterator Find(const Key &key) const
    {
        return this->find(key);
    }

    List<Key> GetKeys() const
    {
        List<Key> result;
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            result.PushBack(it->first);
        }
        return result;
    }

    List<Value> GetValues() const
    {
        List<Value> result;
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            result.PushBack(it->second);
        }
        return result;
    }

    Iterator Begin() { return this->begin(); }
    Iterator End() { return this->end(); }
    Const_Iterator Begin() const { return this->begin(); }
    Const_Iterator End() const { return this->end(); }

    // To allow range-based for loops
    Iterator begin() { return this->std::unordered_map<Key,Value>::begin(); }
    Iterator end() { return this->std::unordered_map<Key,Value>::end(); }
    Const_Iterator begin() const { return this->std::unordered_map<Key,Value>::begin(); }
    Const_Iterator end() const { return this->std::unordered_map<Key,Value>::end(); }

    Value& operator[](const Key &k)
    {
        return std::unordered_map<Key,Value>::operator[](k);
    }
    const Value& operator[](const Key &k) const
    {
        return std::unordered_map<Key,Value>::operator[](k);
    }
};
#endif // UMAP_H
