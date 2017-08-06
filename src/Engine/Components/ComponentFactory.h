#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "Bang/String.h"

FORWARD class Component;

class ComponentFactory
{
public:
    static Component* CreateComponent(const String &componentClassName);

    template<class T, class=T_SUBCLASS(T, Component)>
    static T* CreateComponent();

    ComponentFactory() = delete;
};

template<class T, class>
T* ComponentFactory::CreateComponent()
{
    return new T();
}

#endif // COMPONENTFACTORY_H
