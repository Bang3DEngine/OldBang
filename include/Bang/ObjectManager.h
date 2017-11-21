#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <queue>

#include "Bang/Set.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ICreateListener.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;
FORWARD class ObjectId;
FORWARD class Component;
FORWARD class GameObject;

class ObjectManager : public IDestroyListener,
                      public EventEmitter<ICreateListener>,
                      public EventEmitter<IDestroyListener>
{
public:
    static void RegisterCreateListener(ICreateListener *listener);
    static void UnRegisterCreateListener(ICreateListener *listener);

    static void RegisterDestroyListener(IDestroyListener *listener);
    static void UnRegisterDestroyListener(IDestroyListener *listener);

private:
    std::queue<Object*> m_objectsToBeStartedQueue;
    Set<ObjectId> m_objectsToBeStartedSet;

    std::queue<Object*> m_objectsToBeDestroyedQueue;
    Set<ObjectId> m_objectsDestroyedWhileDestroying;

    ObjectManager() = default;
    virtual ~ObjectManager() = default;

    static void StartObjects();
    static void DestroyObjects();

    template <class ObjectClass, class... Args>
    static ObjectClass* Create(Args... args);

    static void Destroy(GameObject *gameObject);
    static void Destroy(Object *object);

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    void PropagateOnCreated(Object *object);
    void PropagateOnDestroyed(Object *object);

    static ObjectManager *GetInstance();

    #ifdef DEBUG
    public: static bool AssertCreatedFromObjectManager;
    public: static bool AssertDestroyedFromObjectManager;
    #endif

    friend class Asset;
    friend class Window;
    friend class Component;
    friend class Resources;
    friend class GameObject;
    friend class Application;
    friend class SceneManager;
    friend class ComponentFactory;
};

NAMESPACE_BANG_END

#include "Bang/ObjectManager.tcc"

#endif // OBJECTMANAGER_H

