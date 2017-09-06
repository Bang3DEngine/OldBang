#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <iostream>
#include <typeinfo>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

using TypeId = String;

template <class Value>
class TypeMap : public Map<TypeId, Value>
{
public:
    template <class Class>
    void Add(const Value &value = Value())
    {
        Map<TypeId, Value>::Add( typeid(Class).name(), value );
    }
    void Add(const TypeId &className, const Value &value = Value())
    {
        Map<TypeId, Value>::Add( className, value );
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
    Value& Get(const TypeId &className)
    {
        return Map<TypeId, Value>::Get( className );
    }

    template<class Class>
    const Value& Get() const
    {
        return Map<TypeId, Value>::Get( typeid(Class).name() );
    }
    const Value& Get(const TypeId &className) const
    {
        return Map<TypeId, Value>::Get( className );
    }

    template<class Class>
    List<Value> GetAllDerived()
    {
        List<Value> values;
        for (const auto& it : *this)
        {
            Class *v = DCAST<Class*>(it.second);
            if (v) { values.PushBack( SCAST<Class*>(v) ); }
        }
        return values;
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
        return ::std::map<TypeId,Value>::operator[]( typeid(Class).name() );
    }
};

NAMESPACE_BANG_END

#endif // TYPEMAP_H
