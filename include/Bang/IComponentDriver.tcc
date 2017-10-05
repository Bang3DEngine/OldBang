#include "Bang/IComponentDriver.h"

#include <type_traits>

#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

template<class T>
T* IComponentDriver<T>::Create()
{
    GameObject *go = GameObjectFactory::CreateGameObject();
    return T::CreateInto(go);
}

template<class T>
T* IComponentDriver<T>::CreateUI()
{
    GameObject *go = GameObjectFactory::CreateUIGameObject();
    return T::CreateInto(go);
}
