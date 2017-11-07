#ifndef DESTROYMANAGER_H
#define DESTROYMANAGER_H

#include "Bang/Set.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;

class DestroyManager : public IDestroyListener
{
public:
	DestroyManager();
	virtual ~DestroyManager();

    static void Destroy(Object *object);

private:
    Set<Object*> m_objectsToBeDestroyed;

    static void DestroyObjects();

    // IDestroyListener
    virtual void OnBeforeDestroyed(IEventEmitter *destroyedEmitter);

    static DestroyManager *GetInstance();

    friend class SceneManager;
};

NAMESPACE_BANG_END

#endif // DESTROYMANAGER_H

