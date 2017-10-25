#ifndef ICOMPONENTDRIVER_H
#define ICOMPONENTDRIVER_H

#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"

NAMESPACE_BANG_BEGIN

template <class T>
class IComponentDriver
{
protected:
    IComponentDriver() = default;
    virtual ~IComponentDriver() = default;

private:
    static void EnsureCreateIntoExists() { T::CreateInto(nullptr); }

    friend class GameObjectFactory;
};

#include "Bang/IComponentDriver.tcc"

NAMESPACE_BANG_END

#endif  // ICOMPONENTDRIVER_H

