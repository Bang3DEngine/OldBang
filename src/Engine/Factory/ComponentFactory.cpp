#include "Bang/ComponentFactory.h"

#include "Bang/Debug.h"
#include "Bang/Light.h"
#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/UICanvas.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/Component.h"
#include "Bang/PointLight.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIRenderer.h"
#include "Bang/Application.h"
#include "Bang/AudioSource.h"
#include "Bang/UIInputText.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UITextCursor.h"
#include "Bang/UIScrollArea.h"
#include "Bang/AudioListener.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/StreamOperators.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

#define CREATE_COMPONENT(className, ComponentClass) \
    if (className == ComponentClass::GetClassNameStatic()) \
    { return Component::Create<ComponentClass>(); }

#define EXISTS_COMPONENT(componentClassName, ComponentClass) \
    if (componentClassName == ComponentClass::GetClassNameStatic()) \
    { return true; }

Component* ComponentFactory::Create(const String &componentClassName)
{
    CREATE_COMPONENT(componentClassName, Camera);
    CREATE_COMPONENT(componentClassName, UIMask);
    CREATE_COMPONENT(componentClassName, Renderer);
    CREATE_COMPONENT(componentClassName, UICanvas);
    CREATE_COMPONENT(componentClassName, Transform);
    CREATE_COMPONENT(componentClassName, PointLight);
    CREATE_COMPONENT(componentClassName, UIRectMask);
    CREATE_COMPONENT(componentClassName, AudioSource);
    CREATE_COMPONENT(componentClassName, UIInputText);
    CREATE_COMPONENT(componentClassName, LineRenderer);
    CREATE_COMPONENT(componentClassName, MeshRenderer);
    CREATE_COMPONENT(componentClassName, UIScrollArea);
    CREATE_COMPONENT(componentClassName, AudioListener);
    CREATE_COMPONENT(componentClassName, RectTransform);
    CREATE_COMPONENT(componentClassName, UITextRenderer);
    CREATE_COMPONENT(componentClassName, UIImageRenderer);
    CREATE_COMPONENT(componentClassName, DirectionalLight);
    CREATE_COMPONENT(componentClassName, UIVerticalLayout);
    CREATE_COMPONENT(componentClassName, PostProcessEffect);
    CREATE_COMPONENT(componentClassName, BehaviourContainer);
    CREATE_COMPONENT(componentClassName, UIHorizontalLayout);

    Debug_Warn("Please register class '" << componentClassName << "' in "
               "ComponentFactory");

    return nullptr;
}

bool ComponentFactory::Exists(const String &componentClassName)
{
    EXISTS_COMPONENT(componentClassName, Light);
    EXISTS_COMPONENT(componentClassName, Camera);
    EXISTS_COMPONENT(componentClassName, UIMask);
    EXISTS_COMPONENT(componentClassName, Renderer);
    EXISTS_COMPONENT(componentClassName, UICanvas);
    EXISTS_COMPONENT(componentClassName, Transform);
    EXISTS_COMPONENT(componentClassName, PointLight);
    EXISTS_COMPONENT(componentClassName, UIRectMask);
    EXISTS_COMPONENT(componentClassName, UIRenderer);
    EXISTS_COMPONENT(componentClassName, AudioSource);
    EXISTS_COMPONENT(componentClassName, UIInputText);
    EXISTS_COMPONENT(componentClassName, LineRenderer);
    EXISTS_COMPONENT(componentClassName, MeshRenderer);
    EXISTS_COMPONENT(componentClassName, UIScrollArea);
    EXISTS_COMPONENT(componentClassName, AudioListener);
    EXISTS_COMPONENT(componentClassName, RectTransform);
    EXISTS_COMPONENT(componentClassName, UITextRenderer);
    EXISTS_COMPONENT(componentClassName, UIImageRenderer);
    EXISTS_COMPONENT(componentClassName, DirectionalLight);
    EXISTS_COMPONENT(componentClassName, UIVerticalLayout);
    EXISTS_COMPONENT(componentClassName, PostProcessEffect);
    EXISTS_COMPONENT(componentClassName, BehaviourContainer);
    EXISTS_COMPONENT(componentClassName, UIHorizontalLayout);

    return false;
}
