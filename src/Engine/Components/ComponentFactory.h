#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "Bang/String.h"

FORWARD class Component;

class ComponentFactory
{
public:
    static Component* CreateComponent(const String &componentClassName);

    template<class T>
    static T* CreateComponent();

    ComponentFactory() = delete;
};

template<class T>
T* ComponentFactory::CreateComponent()
{
    return new T();
}

#endif // COMPONENTFACTORY_H
