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

        ASSERT(!objectToBeStarted->IsStarted());
        objectToBeStarted->Start();
    }
}

void ObjectManager::DestroyObjects()
{
    ObjectManager *om = ObjectManager::GetInstance();
    while (!om->m_objectsToBeDestroyedQueue.empty())
    {
        Object *objectToBeDestroyed = om->m_objectsToBeDestroyedQueue.front();
        om->m_objectsToBeDestroyedQueue.pop();
        if (!om->m_objectsDestroyedWhileDestroying.Contains(objectToBeDestroyed))
        {
            delete objectToBeDestroyed;
        }
    }
    om->m_objectsDestroyedWhileDestroying.Clear();
    om->m_objectsToBeDestroyedSet.Clear();
}

void ObjectManager::OnBeforeDestroyed(IEventEmitter *destroyedEmitter)
{
    Object *destroyedObject = DCAST<Object*>(destroyedEmitter);
    m_objectsDestroyedWhileDestroying.Add(destroyedObject);
}

ObjectManager *ObjectManager::GetInstance()
{
    return Window::GetCurrent()->GetObjectManager();
}

#ifdef DEBUG
bool ObjectManager::AssertCreatedFromObjectManager = false;
#endif

