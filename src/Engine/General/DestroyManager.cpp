#include "Bang/DestroyManager.h"

#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/IEventEmitter.h"

USING_NAMESPACE_BANG

DestroyManager::DestroyManager()
{
}

DestroyManager::~DestroyManager()
{
}

void DestroyManager::Destroy(Object *object)
{
    DestroyManager *dm = DestroyManager::GetInstance();
    dm->m_objectsToBeDestroyed.Add(object);
}

void DestroyManager::DestroyObjects()
{
    DestroyManager *dm = DestroyManager::GetInstance();
    while (!dm->m_objectsToBeDestroyed.IsEmpty())
    {
        Object *objectToBeDestroyed = *(dm->m_objectsToBeDestroyed.Begin());
        dm->m_objectsToBeDestroyed.Remove( dm->m_objectsToBeDestroyed.Begin() );
        delete objectToBeDestroyed;
    }
}

void DestroyManager::OnBeforeDestroyed(IEventEmitter *destroyedEmitter)
{
    Object *destroyedObject = DCAST<Object*>(destroyedEmitter);
    m_objectsToBeDestroyed.Remove(destroyedObject);
}

DestroyManager *DestroyManager::GetInstance()
{
    return Window::GetCurrent()->GetDestroyManager();
}

