#include "Bang/ObjectManager.h"

#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"

void ObjectManager::Destroy(GameObject *gameObject)
{
    Destroy( Cast<Object*>(gameObject) );
}

void ObjectManager::Destroy(Object *object)
{
    ObjectManager *om = ObjectManager::GetInstance();

    ObjectId objectToBeDestroyedId = object->GetObjectId();
    if (!object->IsWaitingToBeDestroyed() &&
        !om->m_objectsToBeDestroyedSet.Contains(objectToBeDestroyedId))
    {
        om->m_objectsToBeDestroyedSet.Add(objectToBeDestroyedId);
        om->m_objectsToBeDestroyedQueue.push(object);
        om->m_objectsIdsToBeDestroyedQueue.push(objectToBeDestroyedId);

        object->m_waitingToBeDestroyed = true;
        object->BeforeDestroyed();
        om->PropagateOnDestroyed(object);
    }
}

void ObjectManager::RegisterCreateListener(ICreateListener *listener)
{
    ObjectManager::GetInstance()->
            EventEmitter<ICreateListener>::RegisterListener(listener);
}

void ObjectManager::UnRegisterCreateListener(ICreateListener *listener)
{
    ObjectManager::GetInstance()->
            EventEmitter<ICreateListener>::UnRegisterListener(listener);
}

void ObjectManager::RegisterDestroyListener(IDestroyListener *listener)
{
    ObjectManager::GetInstance()->
            EventEmitter<IDestroyListener>::RegisterListener(listener);
}

void ObjectManager::UnRegisterDestroyListener(IDestroyListener *listener)
{
    ObjectManager::GetInstance()->
            EventEmitter<IDestroyListener>::RegisterListener(listener);
}

void ObjectManager::StartObjects()
{
    ObjectManager *om = ObjectManager::GetInstance();
    while (!om->m_objectsToBeStartedQueue.empty())
    {
        Object *objectToBeStarted = om->m_objectsToBeStartedQueue.front();
        ObjectId objToBeStartedId = om->m_objectsIdsToBeStartedQueue.front();
        om->m_objectsToBeStartedQueue.pop();
        om->m_objectsIdsToBeStartedQueue.pop();

        ASSERT(objToBeStartedId.m_id < ObjectId::s_nextObjectId);

        if (!om->m_objectsToBeDestroyedSet.Contains(objToBeStartedId))
        {
            ASSERT(!objectToBeStarted->IsStarted());
            objectToBeStarted->Start();
            om->PropagateOnCreated(objectToBeStarted);
        }
    }
}

void ObjectManager::DestroyObjects()
{
    ObjectManager *om = ObjectManager::GetInstance();
    while (!om->m_objectsToBeDestroyedQueue.empty())
    {
        Object *objectToBeDestroyed = om->m_objectsToBeDestroyedQueue.front();
        ObjectId objectToBeDestroyedId = om->m_objectsIdsToBeDestroyedQueue.front();
        om->m_objectsToBeDestroyedQueue.pop();
        om->m_objectsIdsToBeDestroyedQueue.pop();

        if (!om->m_objectsDestroyedWhileDestroying.Contains(objectToBeDestroyedId))
        {
            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = true;
            #endif

            om->m_objectsToBeDestroyedSet.Add(objectToBeDestroyedId);
            delete objectToBeDestroyed;

            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = false;
            #endif
        }
    }
    om->m_objectsDestroyedWhileDestroying.Clear();
}

void ObjectManager::OnDestroyed(Object *object)
{
    Object *destroyedObject = Cast<Object*>(object);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject->GetObjectId());
}

void ObjectManager::PropagateOnCreated(Object *object)
{
    PROPAGATE(ICreateListener, OnCreated, object);
}

void ObjectManager::PropagateOnDestroyed(Object *object)
{
    PROPAGATE(IDestroyListener, OnDestroyed, object);
}

ObjectManager *ObjectManager::GetInstance()
{
    return Window::GetActive()->GetObjectManager();
}

#ifdef DEBUG
bool ObjectManager::AssertCreatedFromObjectManager = false;
bool ObjectManager::AssertDestroyedFromObjectManager = false;
#endif

