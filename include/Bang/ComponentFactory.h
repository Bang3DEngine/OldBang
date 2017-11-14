#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "Bang/String.h"
#include "Bang/ObjectManager.h"

NAMESPACE_BANG_BEGIN

FORWARD class Component;

class ComponentFactory
{
public:
    template<class T>
    static T* Create();

private:
    static Component* Create(const String &componentClassName);
    static bool Exists(const String &componentClassName);

    ComponentFactory() = delete;

    friend class Component;
    friend class GameObject;
};

template<class T>
T* ComponentFactory::Create()
{
    return ObjectManager::Create<T>();
}

NAMESPACE_BANG_END

#endif // COMPONENTFACTORY_H
