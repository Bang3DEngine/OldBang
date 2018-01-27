#include "Bang/ObjectManager.h"

#include "Bang/Object.h"

USING_NAMESPACE_BANG

template <class ObjectClass, class... Args>
ObjectClass* ObjectManager::Create(Args... args)
{
    ObjectManager *om = ObjectManager::GetActive();

    ObjectClass *newObj = new ObjectClass(args...);

    Object *obj = Cast<ObjectClass*>(newObj);
    om->m_objectsToBeStartedQueue.push(obj);
    om->m_objectsIdsToBeStartedQueue.push(obj->GetObjectId());

    om->EventEmitter<ICreateListener>::
            PropagateToListeners(&ICreateListener::OnCreated, obj);

    return newObj;
}
