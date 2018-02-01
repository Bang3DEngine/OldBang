#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

FORWARD class Component;

class ComponentFactory
{
protected:
    ComponentFactory() = default;
    virtual ~ComponentFactory() = default;

    static Component* Create(const String &componentClassName);
    static bool Exists(const String &componentClassName);

    friend class Component;
    friend class GameObject;
};

NAMESPACE_BANG_END

#endif // COMPONENTFACTORY_H
