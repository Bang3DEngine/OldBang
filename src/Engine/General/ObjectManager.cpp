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
        object->BeforeDestroyed();
        object->m_waitingToBeDestroyed = true;

        om->m_objectsToBeDestroyedQueue.push(object);
        om->m_objectsToBeDestroyedSet.Add(object);
    }
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

void ObjectManager::OnBeforeDestroyed(Object *object)
{
    Object *destroyedObject = DCAST<Object*>(object);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject);
}

ObjectManager *ObjectManager::GetInstance()
{
    return Window::GetActive()->GetObjectManager();
}

#ifdef DEBUG
bool ObjectManager::AssertCreatedFromObjectManager = false;
bool ObjectManager::AssertDestroyedFromObjectManager = false;
#endif

