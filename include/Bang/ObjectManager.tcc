#include "Bang/ObjectManager.h"

USING_NAMESPACE_BANG

template <class ObjectClass, class... Args>
ObjectClass* ObjectManager::Create(Args... args)
{
    ObjectManager *om = ObjectManager::GetInstance();

    #ifdef DEBUG
    ObjectManager::AssertCreatedFromObjectManager = true;
    #endif

    ObjectClass *newObj = new ObjectClass(args...);

    #ifdef DEBUG
    ObjectManager::AssertCreatedFromObjectManager = false;
    #endif

    Object *obj = SCAST<ObjectClass*>(newObj);
    om->m_objectsToBeStartedQueue.push(obj);

    return newObj;
}
