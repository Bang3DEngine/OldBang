#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang/IO.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/Random.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/SingletonManager.h"
#include "Bang/ISceneEventListener.h"


#ifdef BANG_EDITOR
#include "Bang/EditorWindow.h"
#endif

class BehaviourHolder;

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public ISceneEventListener
{

public:
    Behaviour();
    virtual ~Behaviour();

    // Substitute CLASS_NAME
    static String s_behaviourHeaderTemplate;
    static String s_behaviourSourceTemplate;

    String GetBehaviourName() const;

    /**
     * These variables must be copied from BehaviourHolder
     */
    GameObject* m_gameObject = nullptr;
    GameObject* m_parent = nullptr;
    Transform* m_transform = nullptr;

    GameObject* const& gameObject = m_gameObject;
    GameObject* const& parent = m_parent;
    Transform*  const& transform  = m_transform;

    virtual void OnUpdate() override;

/**
 * @brief Set directly by BehaviourHolder (its a friend class), when
 * adding this Behaviour to itself.
 */
private:
    BehaviourHolder *p_behaviourHolder = nullptr;

    void Init(BehaviourHolder *bh);
    virtual void _OnUpdate() override;

    /**
     * @brief Creates a Behaviour from its QLibrary passed as parameter.
     * This will try to call the function CreateDynamically, loading the library
     * passed as parameter. This method CreateDynamically should be defined
     * in the XXXBehaviour.cpp file, as extern "C".
     * @param The dynamic library of the Behaviour that you must have opened before.
     * @return The created Behaviour.
     */
    static Behaviour* CreateDynamicBehaviour(const String &behaviourName,
                                             QLibrary *openLibrary);

    /**
     * @brief Delete dynamic Behaviour from a sharedObject passed as parameter.
     * @param The Behaviour you want to delete
     * @param The library that has been opened for the Behaviour you want to delete.
     * @return Success or not.
     */
    static bool DeleteDynamicBehaviour(const String &behaviourName,
                                       Behaviour *b,
                                       QLibrary *openLibrary);

    friend class BehaviourHolder;
};

#endif


// DEFINES

#define BANG_BEHAVIOUR_CLASS(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically_##CLASS_NAME(\
        SingletonManager *mainBinarySingletonManager); \
extern "C" void DeleteDynamically_##CLASS_NAME(Behaviour *b);

#define BANG_BEHAVIOUR_CLASS_IMPL(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically_##CLASS_NAME(\
        SingletonManager *mainBinarySingletonManager) \
{ \
\
    /* This line links the SingletonManager in the main binary \
        to the SingletonManager in the behaviour loaded library. */ \
    SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(\
                mainBinarySingletonManager); \
\
    return new CLASS_NAME(); \
} \
\
extern "C" void DeleteDynamically_##CLASS_NAME(Behaviour *b) \
{ \
    delete b; \
}















