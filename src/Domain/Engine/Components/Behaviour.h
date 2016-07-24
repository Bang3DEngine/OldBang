#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"

#include "Logger.h"
#include "Transform.h"
#include "GameObject.h"
#include "SingletonManager.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "WindowMain.h"
#include "IWindowEventManagerListener.h"
#endif

class BehaviourHolder;

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public ISceneEventListener
{
friend class BehaviourHolder;

private:

    /**
     * @brief Set directly by BehaviourHolder (its a friend class), when
     * adding this Behaviour to itself.
     */
    BehaviourHolder *m_behaviourHolder = nullptr;


    void Init(BehaviourHolder *bh);
    void SetSourceFilepath(const std::string &m_sourceFilepath);
    std::string GetSourceFilepath() const;

/**
 * These variables must be copied from BehaviourHolder
 */
protected:
    GameObject* m_parent = nullptr;
    GameObject* m_gameObject = nullptr;
    Transform*  m_transform  = nullptr;

protected:
    GameObject* const& gameObject = m_gameObject;
    Transform*  const& transform  = m_transform;
/**
*/

public:
    Behaviour();
    virtual ~Behaviour();

protected:
    virtual void OnUpdate() override;

private:
    virtual void _OnUpdate() override;
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












