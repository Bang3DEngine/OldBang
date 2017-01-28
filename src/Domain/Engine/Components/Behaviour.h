#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Time.h"
#include "Debug.h"
#include "Input.h"
#include "Debug.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"
#include "Persistence.h"
#include "SceneManager.h"
#include "ShaderContract.h"
#include "SingletonManager.h"
#include "ISceneEventListener.h"


#ifdef BANG_EDITOR
#include "EditorWindow.h"
#include "IWindowEventManagerListener.h"
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

/**
 * These variables must be copied from BehaviourHolder
 */
protected:
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
    BehaviourHolder *m_behaviourHolder = nullptr;

    void Init(BehaviourHolder *bh);
    void SetSourceFilepath(const String &m_sourceFilepath);
    String GetSourceFilepath() const;

    virtual void _OnUpdate() override;

    friend class BehaviourHolder;
};

#endif


// DEFINES

#define BANG_BEHAVIOUR_CLASS(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically(SingletonManager *mainBinarySingletonManager) \
{ \
\
    /* This line links the SingletonManager in the main binary to the SingletonManager \
       in the behaviour loaded library. */ \
    SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(mainBinarySingletonManager); \
\
    Behaviour *b = new CLASS_NAME(); \
    return b; \
} \
\
extern "C" void DeleteDynamically(Behaviour *b) \
{ \
    delete b; \
}












