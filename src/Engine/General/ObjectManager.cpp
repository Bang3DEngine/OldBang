#include "Bang/ObjectManager.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/IEventEmitter.h"

bool ObjectManager::AssertCreatedFromObjectManager = false;
bool ObjectManager::AssertDestroyedFromObjectManager = false;

ObjectManager::~ObjectManager()
{
    DestroyObjects();
}

void ObjectManager::Destroy(GameObject *gameObject)
{
    Destroy( Cast<Object*>(gameObject) );
}

void ObjectManager::Destroy(Object *object)
{
    ObjectManager *om = ObjectManager::GetActive();
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
    ObjectManager::GetActive()->
            EventEmitter<ICreateListener>::RegisterListener(listener);
}

void ObjectManager::UnRegisterCreateListener(ICreateListener *listener)
{
    ObjectManager::GetActive()->
            EventEmitter<ICreateListener>::UnRegisterListener(listener);
}

void ObjectManager::RegisterDestroyListener(IDestroyListener *listener)
{
    ObjectManager::GetActive()->
            EventEmitter<IDestroyListener>::RegisterListener(listener);
}

void ObjectManager::UnRegisterDestroyListener(IDestroyListener *listener)
{
    ObjectManager::GetActive()->
            EventEmitter<IDestroyListener>::RegisterListener(listener);
}

void ObjectManager::StartObjects()
{
    Array<Object*> objectsToBeStartedNow;
    while (!m_objectsToBeStartedQueue.empty())
    {
        Object *objectToBeStarted = m_objectsToBeStartedQueue.front();
        ObjectId objToBeStartedId = m_objectsIdsToBeStartedQueue.front();
        m_objectsToBeStartedQueue.pop();
        m_objectsIdsToBeStartedQueue.pop();

        ASSERT(objToBeStartedId.m_id < ObjectId::NextObjectId);

        if (!m_objectsToBeDestroyedSet.Contains(objToBeStartedId))
        {
            ASSERT(!objectToBeStarted->IsStarted());
            ASSERT(!objectToBeStarted->IsWaitingToBeDestroyed());
            objectsToBeStartedNow.PushBack(objectToBeStarted);
        }
    }

    for (Object *obj : objectsToBeStartedNow) { obj->PreStart(); }
    for (Object *obj : objectsToBeStartedNow) { obj->Start(); }
}

void ObjectManager::DestroyObjects()
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

void ObjectManager::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    Object *destroyedObject = SCAST<Object*>(object);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject->GetObjectId());
}

ObjectManager *ObjectManager::GetActive()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetLocalObjectManager() :
                   Window::GetActive()->GetGlobalObjectManager();
}

