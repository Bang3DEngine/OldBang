#include "Bang/ObjectManager.h"

#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"

ObjectManager::~ObjectManager()
{
    _DestroyObjects();
}

void ObjectManager::Destroy(GameObject *gameObject)
{
    Destroy( Cast<Object*>(gameObject) );
}

void ObjectManager::Destroy(Object *object)
{
    ObjectManager *om = ObjectManager::GetInstance();
    if (om)
    {
        ObjectId objectToBeDestroyedId = object->GetObjectId();
        if (!object->IsWaitingToBeDestroyed() &&
            !om->m_objectsToBeDestroyedSet.Contains(objectToBeDestroyedId))
        {
            om->m_objectsToBeDestroyedSet.Add(objectToBeDestroyedId);
            om->m_objectsToBeDestroyedQueue.push(object);
            om->m_objectsIdsToBeDestroyedQueue.push(objectToBeDestroyedId);

            object->m_waitingToBeDestroyed = true;
            object->BeforeDestroyed();
            om->EventEmitter<IDestroyListener>::
                    PropagateToListeners(&IDestroyListener::OnDestroyed, object);
        }
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

        ASSERT(objToBeStartedId.m_id < ObjectId::NextObjectId);

        if (!om->m_objectsToBeDestroyedSet.Contains(objToBeStartedId))
        {
            ASSERT(!objectToBeStarted->IsStarted());
            ASSERT(!objectToBeStarted->IsWaitingToBeDestroyed());
            objectToBeStarted->Start();
        }
    }
}

void ObjectManager::DestroyObjects()
{
    ObjectManager *om = ObjectManager::GetInstance();
    om->_DestroyObjects();
}

void ObjectManager::_DestroyObjects()
{
    while (!m_objectsToBeDestroyedQueue.empty())
    {
        Object *objectToBeDestroyed = m_objectsToBeDestroyedQueue.front();
        ObjectId objectToBeDestroyedId = m_objectsIdsToBeDestroyedQueue.front();
        ASSERT(objectToBeDestroyed->GetObjectId() == objectToBeDestroyedId);
        ASSERT(objectToBeDestroyed->IsWaitingToBeDestroyed());
        m_objectsToBeDestroyedQueue.pop();
        m_objectsIdsToBeDestroyedQueue.pop();

        if (!m_objectsDestroyedWhileDestroying.Contains(objectToBeDestroyedId))
        {
            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = true;
            #endif

            m_objectsToBeDestroyedSet.Add(objectToBeDestroyedId);
            delete objectToBeDestroyed;

            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = false;
            #endif
        }
    }
    m_objectsDestroyedWhileDestroying.Clear();
}

void ObjectManager::OnDestroyed(Object *object)
{
    Object *destroyedObject = Cast<Object*>(object);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject->GetObjectId());
}

ObjectManager *ObjectManager::GetInstance()
{
    return Window::GetActive()->GetObjectManager();
}

#ifdef DEBUG
bool ObjectManager::AssertCreatedFromObjectManager = false;
bool ObjectManager::AssertDestroyedFromObjectManager = false;
#endif

