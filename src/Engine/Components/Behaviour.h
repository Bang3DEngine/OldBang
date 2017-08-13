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
#include "Bang/Resources.h"
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

    void RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour = nullptr);
    static Behaviour* CreateDynamicBehaviour(const String &behaviourName,
                                             QLibrary *openLibrary);
    static bool DeleteDynamicBehaviour(const String &behaviourName,
                                       Behaviour *behaviour,
                                       QLibrary *openLibrary);

    void SetSourceFilepath(const Path &sourceFilepath);

    const Path& GetSourceFilepath() const;
    bool IsLoaded() const;

    // Component
    virtual void Start () override;
    virtual void Update() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
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
