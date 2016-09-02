#ifndef MAP_H
#define MAP_H

#include <map>
#include "List.h"
#include <sstream>

#include "List.h"
#include "String.h"

template <class Key, class Value>
class Map : public std::map<Key, Value>
{
public:
    typedef typename std::map<Key, Value>::iterator Iterator;
    typedef typename std::map<Key, Value>::const_iterator Const_Iterator;
    typedef typename std::map<Key, Value>::reverse_iterator Reverse_Iterator;
    typedef typename std::map<Key, Value>::const_reverse_iterator Const_Reverse_Iterator;

    Map()
    {
    }

    Map(const std::map<Key, Value> &m) : std::map<Key, Value>(m)
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
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (it->second == value) return true;
        }
        return false;
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

    Iterator Begin()
    {
        return this->begin();
    }
    Iterator End()
    {
        return this->end();
    }
    Const_Iterator Begin() const
    {
        return this->begin();
    }
    Const_Iterator End() const
    {
        return this->end();
    }
    Reverse_Iterator RBegin()
    {
        return this->rbegin();
    }
    Reverse_Iterator REnd()
    {
        return this->rend();
    }
    Const_Reverse_Iterator RBegin() const
    {
        return this->rbegin();
    }
    Const_Reverse_Iterator REnd() const
    {
        return this->rend();
    }

    String ToString()
    {
        std::ostringstream oss;
        oss << "{";
        for (auto it = this->Begin(); it != this->End(); ++it)
        {
            if (it != this->Begin()) oss << ", ";
            oss << (it->first) << ": " << (it->second);
        }
        oss << "}";
        return oss.str();
    }
};

#endif // MAP_H
