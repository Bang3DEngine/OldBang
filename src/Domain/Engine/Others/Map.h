#ifndef MAP_H
#define MAP_H

#include <map>
#include <list>
#include <sstream>

#include "String.h"

template <class Key, class Value>
class Map : private std::map<Key, Value>
{
public:
    typedef typename std::map<Key, Value>::iterator Iterator;
    typedef typename std::map<Key, Value>::const_iterator Const_Iterator;

    Map()
    {
    }

    void Add(const Key &key, const Value &value)
    {
        (*this)[key] = value;
    }

    void Remove(const Key &key)
    {
        this->erase(key);
    }

    const Value& Get(const Key &key) const
    {
        Map<Key, Value> *noConstThis = const_cast< Map<Key, Value>* >(this);
        return (*noConstThis)[key];
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
        return this->find(key) != this->end();
    }

    bool ContainsValue(const Value &value) const
    {
        for (auto it = this->begin(); it != this->end(); ++it)
        {
            if (it->second == value) return true;
        }
        return false;
    }

    std::list<Key> GetKeys() const
    {
        std::list<Key> result;
        for (auto it = this->begin(); it != this->end(); ++it)
        {
            result.push_back(it->first);
        }
        return result;
    }

    std::list<Value> GetValues() const
    {
        std::list<Value> result;
        for (auto it = this->begin(); it != this->end(); ++it)
        {
            result.push_back(it->second);
        }
        return result;
    }

    typename std::map<Key, Value>::const_iterator Begin()
    {
        return this->begin();
    }
    typename std::map<Key, Value>::const_iterator End()
    {
        return this->end();
    }

    String ToString()
    {
        std::ostringstream oss;
        oss << "{";
        for (auto it = this->begin(); it != this->end(); ++it)
        {
            if (it != this->begin()) oss << ", ";
            oss << (it->first) << ": " << (it->second);
        }
        oss << "}";
        return oss.str();
    }
};

#endif // MAP_H
