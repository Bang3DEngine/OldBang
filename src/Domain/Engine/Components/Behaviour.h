#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <QLibrary>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/UIText.h"
#include "Bang/Random.h"
#include "Bang/Camera.h"
#include "Bang/UIImage.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/DirectionalLight.h"
#include "Bang/SingletonManager.h"
#include "Bang/ISceneEventListener.h"

/**
 * @brief The Behaviour class is a base class which will be inherited by all
 * the User Behaviour classes. This lets the engine handle Behaviours uniformly.
 */
class Behaviour : public Component
                  #ifdef BANG_EDITOR
                  ,public IAttrWidgetButtonListener
                  #endif
{
    OBJECT(Behaviour)
    COMPONENT_ICON(Behaviour, "Icons/BehaviourIcon.png")

public:
    Behaviour();
    virtual ~Behaviour();

    virtual void _OnStart () override;
    virtual void _OnUpdate() override;

    virtual void CloneInto(ICloneable *clone) const override;

    const Path& GetSourceFilepath() const;
    virtual void OnAddedToGameObject() override;
    void RefreshBehaviourLib();

    bool IsLoaded() const;

    #ifdef BANG_EDITOR
    void OnButtonClicked(const String &attrName) override;
    static Behaviour* CreateNewBehaviour();
    #endif

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

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    static String s_behaviourHeaderTemplate;
    static String s_behaviourSourceTemplate;

    Path m_sourceFilepath;
    QLibrary *p_behavioursLibraryBeingUsed = nullptr;
};

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

#endif
