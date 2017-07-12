#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/UIText.h"
#include "Bang/Camera.h"
#include "Bang/UIImage.h"
#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/Transform.h"
#include "Bang/AudioSource.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioClipFile.h"
#include "Bang/RectTransform.h"
#include "Bang/CircleRenderer.h"
#include "Bang/ComponentWidget.h"
#include "Bang/DirectionalLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/UITextInspectorWidget.h"
#include "Bang/CameraInspectorWidget.h"
#include "Bang/UIImageInspectorWidget.h"
#include "Bang/BehaviourInspectorWidget.h"
#include "Bang/TransformInspectorWidget.h"
#include "Bang/PointLightInspectorWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/MeshRendererInspectorWidget.h"
#include "Bang/LineRendererInspectorWidget.h"
#include "Bang/RectTransformInspectorWidget.h"
#include "Bang/AudioListenerInspectorWidget.h"
#include "Bang/AudioClipFileInspectorWidget.h"
#include "Bang/CircleRendererInspectorWidget.h"
#include "Bang/DirectionalLightInspectorWidget.h"
#include "Bang/PostProcessEffectInspectorWidget.h"

List<InspectorWidget*> InspectorWidgetFactory::CreateWidgets(GameObject *go)
{
    List<InspectorWidget*> componentWidgets;
    for (Component *comp : go->GetComponents())
    {
        InspectorWidget *compIW = InspectorWidgetFactory::CreateWidget(comp);
        componentWidgets.PushBack(compIW);
    }
    return componentWidgets;
}

InspectorWidget* InspectorWidgetFactory::CreateWidget(Component *component)
{
    ComponentWidget *compWidget = nullptr;
    if (component->IsOfType<AudioSource>())
    {
        AudioSource *as = static_cast<AudioSource*>(component);
        compWidget = new AudioSourceInspectorWidget(as);
    }
    else if (component->IsOfType<AudioListener>())
    {
        AudioListener *al = static_cast<AudioListener*>(component);
        compWidget = new AudioListenerInspectorWidget(al);
    }
    else if (component->IsOfType<Behaviour>())
    {
        Behaviour *b = static_cast<Behaviour*>(component);
        compWidget = new BehaviourInspectorWidget(b);
    }
    else if (component->IsOfType<Camera>())
    {
        Camera *c = static_cast<Camera*>(component);
        compWidget = new CameraInspectorWidget(c);
    }
    else if (component->IsOfType<UIImage>())
    {
        UIImage *img = static_cast<UIImage*>(component);
        compWidget = new UIImageInspectorWidget(img);
    }
    else if (component->IsOfType<UIText>())
    {
        UIText *txt = static_cast<UIText*>(component);
        compWidget = new UITextInspectorWidget(txt);
    }
    else if (component->IsOfType<CircleRenderer>())
    {
        CircleRenderer *cr = static_cast<CircleRenderer*>(component);
        compWidget = new CircleRendererInspectorWidget(cr);
    }
    else if (component->IsOfType<LineRenderer>())
    {
        LineRenderer *lr = static_cast<LineRenderer*>(component);
        compWidget = new LineRendererInspectorWidget(lr);
    }
    else if (component->IsOfType<MeshRenderer>())
    {
        MeshRenderer *mr = static_cast<MeshRenderer*>(component);
        compWidget = new MeshRendererInspectorWidget(mr);
    }
    else if (component->IsOfType<DirectionalLight>())
    {
        DirectionalLight *dl = static_cast<DirectionalLight*>(component);
        compWidget = new DirectionalLightInspectorWidget(dl);
    }
    else if (component->IsOfType<PointLight>())
    {
        PointLight *pl = static_cast<PointLight*>(component);
        compWidget = new PointLightInspectorWidget(pl);
    }
    else if (component->IsOfType<RectTransform>())
    {
        RectTransform *rt = static_cast<RectTransform*>(component);
        compWidget = new RectTransformInspectorWidget(rt);
    }
    else if (component->IsOfType<Transform>())
    {
        Transform *tr = static_cast<Transform*>(component);
        compWidget = new TransformInspectorWidget(tr);
    }
    else if (component->IsOfType<PostProcessEffect>())
    {
        PostProcessEffect *ppe = static_cast<PostProcessEffect*>(component);
        compWidget = new PostProcessEffectInspectorWidget(ppe);
    }
    else
    {
        compWidget = new ComponentWidget(component);
    }

    compWidget->Init();
    compWidget->Refresh();
    return compWidget;
}

InspectorWidget* InspectorWidgetFactory::CreateWidget(const BFile &file)
{
    InspectorWidget *inspWidget = nullptr;
    if ( Object::IsOfType<AudioClipFile>(&file) )
    {
        const AudioClipFile &audioFile = static_cast<const AudioClipFile&>(file);
        inspWidget = new AudioClipFileInspectorWidget(audioFile);
    }
    else
    {
        inspWidget = new InspectorWidget();
    }

    inspWidget->Init();
    inspWidget->Refresh();
    return inspWidget;
}
