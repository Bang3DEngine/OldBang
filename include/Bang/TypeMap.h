#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <iostream>
#include <typeinfo>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

using TypeId = String;
template <class T>
static TypeId GetTypeId() { return typeid(T).name(); }
template <class T>
static TypeId GetTypeId(const T& x) { return typeid(x).name(); }

template <class Value>
class TypeMap : public Map<TypeId, Value>
{
public:
    template <class Class>
    void Add(const Value &value = Value())
    {
        Map<TypeId, Value>::Add( GetTypeId<Class>(), value );
    }
    void Add(const TypeId &className, const Value &value = Value())
    {
        Map<TypeId, Value>::Add( className, value );
    }

    template <class Class>
    void Remove()
    {
        Map<TypeId, Value>::Remove( GetTypeId<Class>() );
    }

    template<class Class>
    Value& Get()
    {
        return Map<TypeId, Value>::Get( GetTypeId<Class>() );
    }
    Value& Get(const TypeId &className)
    {
        return Map<TypeId, Value>::Get( className );
    }

    template<class Class>
    const Value& Get() const
    {
        return Map<TypeId, Value>::Get( GetTypeId<Class>() );
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
            Class *v = Cast<Class*>(it.second);
            if (v) { values.PushBack( Cast<Class*>(v) ); }
        }
        return values;
    }

    template<class Class>
    bool ContainsKey() const
    {
        return ContainsKey( GetTypeId<Class>() );
    }
    bool ContainsKey(const TypeId &typeId) const
    {
        return Map<TypeId,Value>::ContainsKey( typeId );
    }
};

NAMESPACE_BANG_END

#endif // TYPEMAP_H
