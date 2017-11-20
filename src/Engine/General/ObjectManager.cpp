#include "Bang/ObjectManager.h"

#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"

void ObjectManager::Destroy(GameObject *gameObject)
{
    Destroy( SCAST<Object*>(gameObject) );
}

void ObjectManager::Destroy(Object *object)
{
    ObjectManager *om = ObjectManager::GetInstance();

    if (!om->m_objectsToBeDestroyedSet.Contains(object))
    {
        om->m_objectsToBeDestroyedQueue.push(object); // Must go before
        om->m_objectsToBeDestroyedSet.Add(object);    // Must go before

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
        om->m_objectsToBeStartedQueue.pop();

        if (!om->m_objectsDestroyedInLastFrame.Contains(objectToBeStarted))
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
    om->m_objectsDestroyedInLastFrame.Clear();
    while (!om->m_objectsToBeDestroyedQueue.empty())
    {
        Object *objectToBeDestroyed = om->m_objectsToBeDestroyedQueue.front();
        om->m_objectsToBeDestroyedQueue.pop();

        if (!om->m_objectsDestroyedWhileDestroying.Contains(objectToBeDestroyed))
        {
            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = true;
            #endif

            om->m_objectsDestroyedInLastFrame.Add(objectToBeDestroyed);

            delete objectToBeDestroyed;

            #ifdef DEBUG
            ObjectManager::AssertDestroyedFromObjectManager = false;
            #endif
        }
    }
    om->m_objectsDestroyedWhileDestroying.Clear();
    om->m_objectsToBeDestroyedSet.Clear();
}

void ObjectManager::OnDestroyed(Object *object)
{
    Object *destroyedObject = DCAST<Object*>(object);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject);
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

