#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <QLibrary>

#include "Bang/Paths.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/Random.h"
#include "Bang/Camera.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/SceneNode.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public Component
{
    COMPONENT(Behaviour)

public:
    Behaviour();
    virtual ~Behaviour();

    virtual void Start () override;
    virtual void Update() override;

    virtual void CloneInto(ICloneable *clone) const override;

    const Path& GetSourceFilepath() const;
    void RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour = nullptr);

    bool IsLoaded() const;

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
                                       Behaviour *behaviour,
                                       QLibrary *openLibrary);

    void SetSourceFilepath(const Path &sourceFilepath);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    Path m_sourceFilepath;
    QLibrary *p_behavioursLibraryBeingUsed = nullptr;
};

// DEFINES
#define BANG_BEHAVIOUR_CLASS(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically_##CLASS_NAME(\
        Application *mainBinaryApplication); \
extern "C" void DeleteDynamically_##CLASS_NAME(Behaviour *b);

#define BANG_BEHAVIOUR_CLASS_IMPL(CLASS_NAME) \
extern "C" Behaviour *CreateDynamically_##CLASS_NAME(\
        Application *mainBinaryApplication) \
{ \
\
    /* This line links the Application in the main binary \
        to the Application in the behaviour loaded library. */ \
    Application::SetApplicationSingleton(mainBinaryApplication); \
    return new CLASS_NAME(); \
} \
\
extern "C" void DeleteDynamically_##CLASS_NAME(Behaviour *b) \
{ \
    delete b; \
}

#endif
