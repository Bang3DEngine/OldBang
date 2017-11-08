#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <queue>

#include "Bang/Set.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;
FORWARD class Component;
FORWARD class GameObject;

class ObjectManager : public IDestroyListener
{
public:
    template <class ObjectClass, class... Args>
    static ObjectClass* Create(Args... args);

    static void Destroy(GameObject *gameObject);
    static void Destroy(Object *object);

private:
    std::queue<Object*> m_objectsToBeStartedQueue;
    std::queue<Object*> m_objectsToBeDestroyedQueue;
    Set<Object*> m_objectsToBeDestroyedSet;
    Set<Object*> m_objectsDestroyedWhileDestroying;

    ObjectManager() = default;
    virtual ~ObjectManager() = default;

    static void StartObjects();
    static void DestroyObjects();

    // IDestroyListener
    virtual void OnBeforeDestroyed(Object *object) override;

    static ObjectManager *GetInstance();

    #ifdef DEBUG
    public: static bool AssertCreatedFromObjectManager;
    #endif

    friend class Window;
    friend class Application;
    friend class SceneManager;
    friend class ComponentFactory;
    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#include "Bang/ObjectManager.tcc"

#endif // OBJECTMANAGER_H

