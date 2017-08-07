#include "Bang/ComponentFactory.h"

#include "Bang/Light.h"
#include "Bang/Camera.h"
#include "Bang/UIButton.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/Component.h"
#include "Bang/Behaviour.h"
#include "Bang/UIRenderer.h"
#include "Bang/PointLight.h"
#include "Bang/AudioSource.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UIBorderRect.h"
#include "Bang/RectTransform.h"
#include "Bang/AudioListener.h"
#include "Bang/UIButtonTinter.h"
#include "Bang/CircleRenderer.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/SingleLineRenderer.h"

#define HANDLE_COMPONENT(className, ComponentClass) \
    if (className == ComponentClass::GetClassNameStatic()) \
    { return new ComponentClass(); }

Component* ComponentFactory::CreateComponent(const String &componentClassName)
{
    HANDLE_COMPONENT(componentClassName, AudioListener);
    HANDLE_COMPONENT(componentClassName, AudioSource);
    HANDLE_COMPONENT(componentClassName, Behaviour);
    HANDLE_COMPONENT(componentClassName, Camera);
    HANDLE_COMPONENT(componentClassName, CircleRenderer);
    HANDLE_COMPONENT(componentClassName, DirectionalLight);
    HANDLE_COMPONENT(componentClassName, Light);
    HANDLE_COMPONENT(componentClassName, LineRenderer);
    HANDLE_COMPONENT(componentClassName, MeshRenderer);
    HANDLE_COMPONENT(componentClassName, PointLight);
    HANDLE_COMPONENT(componentClassName, PostProcessEffect);
    HANDLE_COMPONENT(componentClassName, RectTransform);
    HANDLE_COMPONENT(componentClassName, Renderer);
    HANDLE_COMPONENT(componentClassName, SingleLineRenderer);
    HANDLE_COMPONENT(componentClassName, Transform);
    HANDLE_COMPONENT(componentClassName, UIImageRenderer);
    HANDLE_COMPONENT(componentClassName, UIRenderer);
    HANDLE_COMPONENT(componentClassName, UIButton);
    HANDLE_COMPONENT(componentClassName, UIBorderRect);
    HANDLE_COMPONENT(componentClassName, UIButtonTinter);
    HANDLE_COMPONENT(componentClassName, UITextRenderer);

    Debug_Error("Please register class '" << componentClassName << "' in "
                "ComponentFactory"); ASSERT(false);
    return nullptr;
}
