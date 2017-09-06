#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang/Paths.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Object.h"
#include "Bang/Prefab.h"
#include "Bang/Random.h"
#include "Bang/Camera.h"
#include "Bang/Library.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"


NAMESPACE_BANG_BEGIN

class Behaviour : public Component
{
    COMPONENT(Behaviour)

public:
    Behaviour();
    virtual ~Behaviour();

    void RefreshBehaviourLib(const XMLNode *xmlInfoForNewBehaviour = nullptr);
    static Behaviour* CreateDynamicBehaviour(const String &behaviourName,
                                             Library *openLibrary);
    static bool DeleteDynamicBehaviour(const String &behaviourName,
                                       Behaviour *behaviour,
                                       Library *openLibrary);

    void SetSourceFilepath(const Path &sourceFilepath);

    const Path& GetSourceFilepath() const;
    bool IsLoaded() const;

    // Component
    virtual void OnStart () override;
    virtual void OnUpdate() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Path m_sourceFilepath;
    Library *p_behavioursLibraryBeingUsed = nullptr;
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

NAMESPACE_BANG_END

#endif
