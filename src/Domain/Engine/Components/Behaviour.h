#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"
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
    /**
     * @brief Filepath of the source code (*.cpp usually).
     * Just for info purposes for the BehaviourHolder.
     * Semantically it makes more sense to me saving it here.
     */
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
