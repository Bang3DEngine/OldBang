#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"
#include "Logger.h"
#include "WindowMain.h"
#include "ISceneEventListener.h"
#include "IWindowEventManagerListener.h"

class BehaviourHolder;

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public ISceneEventListener
                 ,public IWindowEventManagerListener  // To receive events!
{
friend class BehaviourHolder;

private:

    std::string sourceFilepath = "";

    /**
     * @brief Set directly by BehaviourHolder (its a friend class), when
     * adding this Behaviour to itself.
     */
    BehaviourHolder *behaviourHolder = nullptr;

public:
    Behaviour();
    virtual ~Behaviour();

    /**
     * @brief Remember that Behaviour is not a Component.
     * However, we need to give the user access to the gameObject which has the
     * BehaviourHolder which has this Behaviour...
     * @return The owner of the BehaviourHolder of this Behaviour
     */
    GameObject* GetOwner() const;

    void SetSourceFilepath(const std::string &sourceFilepath);
    std::string GetSourceFilepath() const;
};

#endif


// DEFINES

#define BANG_BEHAVIOUR_CLASS(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically(WindowMain *singletonWindowMain) \
{ \
    Logger_Log("Creating behaviour dinamycally..."); \
\
    WindowMain::Init(singletonWindowMain); \
    Behaviour *b = new CLASS_NAME(); \
\
    Logger_Log("Dynamic behaviour successfully created!"); \
    return b; \
} \
\
extern "C" void DeleteDynamically(int *b) \
{ \
    delete b; \
}












