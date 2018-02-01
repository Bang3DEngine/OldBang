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
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

NAMESPACE_BANG_BEGIN

class Behaviour : public Component
{
    COMPONENT(Behaviour)

public:
    Behaviour();
    virtual ~Behaviour();
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
    return Component::Create<CLASS_NAME>(); \
} \
\
extern "C" void DeleteDynamically_##CLASS_NAME(Behaviour *b) \
{ \
    delete b; \
}

NAMESPACE_BANG_END

#endif
