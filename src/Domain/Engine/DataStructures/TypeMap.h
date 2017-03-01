#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <iostream>
#include <typeinfo>

#include "Map.h"
#include "String.h"

typedef String TypeId;
template <class Value>
class TypeMap : public Map<TypeId, Value>
{
public:
    template <class Class>
    void Set(const Value &value)
    {
        Map<TypeId, Value>::Set( typeid(Class).name(), value );
    }

    template <class Class>
    void Remove()
    {
        Map<TypeId, Value>::Remove( typeid(Class).name() );
    }

    template<class Class>
    Value& Get()
    {
        return Map<TypeId, Value>::Get( typeid(Class).name() );
    }
    template<class Class>
    const Value& Get() const
    {
        return Map<TypeId, Value>::Get( typeid(Class).name() );
    }

    template<class Class>
    bool ContainsKey() const
    {
        return Map<TypeId,Value>::ContainsKey( typeid(Class).name() );
    }

    template<class Class>
    Value& operator[](const Class &k)
    {
        return Map<TypeId,Value>::operator[]( typeid(Class).name() );
    }
    template<class Class>
    const Value& operator[](const Class &k) const
    {
        return std::map<TypeId,Value>::operator[]( typeid(Class).name() );
    }
};

#endif // TYPEMAP_H
