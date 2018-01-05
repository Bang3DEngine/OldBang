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

protected:
    ComponentFactory() = default;
    virtual ~ComponentFactory() = default;

protected:
    static Component* Create(const String &componentClassName);
    static bool Exists(const String &componentClassName);

    virtual Component* _Create(const String &componentClassName);
    virtual bool _Exists(const String &componentClassName);

    static ComponentFactory* GetInstance();

    friend class Component;
    friend class GameObject;
    friend class Application;
};

template<class T>
T* ComponentFactory::Create()
{
    return ObjectManager::Create<T>();
}

NAMESPACE_BANG_END

#endif // COMPONENTFACTORY_H
