#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/UIText.h"
#include "Bang/Camera.h"
#include "Bang/UIImage.h"
#include "Bang/FontFile.h"
#include "Bang/ImageFile.h"
#include "Bang/Transform.h"
#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/MaterialFile.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioClipFile.h"
#include "Bang/RectTransform.h"
#include "Bang/CircleRenderer.h"
#include "Bang/ComponentWidget.h"
#include "Bang/DirectionalLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/FontInspectorWidget.h"
#include "Bang/ImageInspectorWidget.h"
#include "Bang/UITextInspectorWidget.h"
#include "Bang/CameraInspectorWidget.h"
#include "Bang/UIImageInspectorWidget.h"
#include "Bang/MaterialInspectorWidget.h"
#include "Bang/BehaviourInspectorWidget.h"
#include "Bang/TransformInspectorWidget.h"
#include "Bang/AudioClipInspectorWidget.h"
#include "Bang/PointLightInspectorWidget.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/MeshRendererInspectorWidget.h"
#include "Bang/LineRendererInspectorWidget.h"
#include "Bang/RectTransformInspectorWidget.h"
#include "Bang/AudioListenerInspectorWidget.h"
#include "Bang/CircleRendererInspectorWidget.h"
#include "Bang/DirectionalLightInspectorWidget.h"
#include "Bang/PostProcessEffectInspectorWidget.h"

List<InspectorWidget*> InspectorWidgetFactory::CreateWidgets(GameObject *go)
{    List<InspectorWidget*> componentWidgets;
    for (Component *comp : go->GetComponents())
    {
        InspectorWidget *compIW = InspectorWidgetFactory::CreateWidget(comp);
        componentWidgets.PushBack(compIW);
    }
    return componentWidgets;
}

InspectorWidget* InspectorWidgetFactory::CreateWidget(Object *obj)
{
    InspectorWidget *inspWidget = nullptr;
    if (obj->IsOfType<Component>())
    {
        if (obj->IsOfType<AudioSource>())
        {
            AudioSource *as = static_cast<AudioSource*>(obj);
            inspWidget = new AudioSourceInspectorWidget(as);
        }
        else if (obj->IsOfType<AudioListener>())
        {
            AudioListener *al = static_cast<AudioListener*>(obj);
            inspWidget = new AudioListenerInspectorWidget(al);
        }
        else if (obj->IsOfType<Behaviour>())
        {
            Behaviour *b = static_cast<Behaviour*>(obj);
            inspWidget = new BehaviourInspectorWidget(b);
        }
        else if (obj->IsOfType<Camera>())
        {
            Camera *c = static_cast<Camera*>(obj);
            inspWidget = new CameraInspectorWidget(c);
        }
        else if (obj->IsOfType<UIImage>())
        {
            UIImage *img = static_cast<UIImage*>(obj);
            inspWidget = new UIImageInspectorWidget(img);
        }
        else if (obj->IsOfType<UIText>())
        {
            UIText *txt = static_cast<UIText*>(obj);
            inspWidget = new UITextInspectorWidget(txt);
        }
        else if (obj->IsOfType<CircleRenderer>())
        {
            CircleRenderer *cr = static_cast<CircleRenderer*>(obj);
            inspWidget = new CircleRendererInspectorWidget(cr);
        }
        else if (obj->IsOfType<LineRenderer>())
        {
            LineRenderer *lr = static_cast<LineRenderer*>(obj);
            inspWidget = new LineRendererInspectorWidget(lr);
        }
        else if (obj->IsOfType<MeshRenderer>())
        {
            MeshRenderer *mr = static_cast<MeshRenderer*>(obj);
            inspWidget = new MeshRendererInspectorWidget(mr);
        }
        else if (obj->IsOfType<DirectionalLight>())
        {
            DirectionalLight *dl = static_cast<DirectionalLight*>(obj);
            inspWidget = new DirectionalLightInspectorWidget(dl);
        }
        else if (obj->IsOfType<PointLight>())
        {
            PointLight *pl = static_cast<PointLight*>(obj);
            inspWidget = new PointLightInspectorWidget(pl);
        }
        else if (obj->IsOfType<RectTransform>())
        {
            RectTransform *rt = static_cast<RectTransform*>(obj);
            inspWidget = new RectTransformInspectorWidget(rt);
        }
        else if (obj->IsOfType<Transform>())
        {
            Transform *tr = static_cast<Transform*>(obj);
            inspWidget = new TransformInspectorWidget(tr);
        }
        else if (obj->IsOfType<PostProcessEffect>())
        {
            PostProcessEffect *ppe = static_cast<PostProcessEffect*>(obj);
            inspWidget = new PostProcessEffectInspectorWidget(ppe);
        }
        else
        {
            inspWidget = new ComponentWidget( static_cast<Component*>(obj) );
        }
    }
    else if (obj->IsOfType<BFile>())
    {
        if ( Object::IsOfType<AudioClipFile>(obj) )
        {
            AudioClipFile *acf = static_cast<AudioClipFile*>(obj);
            inspWidget = new AudioClipInspectorWidget(*acf);
        }
        else if ( Object::IsOfType<FontFile>(obj) )
        {
            FontFile *ff = static_cast<FontFile*>(obj);
            inspWidget = new FontInspectorWidget(*ff);
        }
        else if ( Object::IsOfType<ImageFile>(obj) )
        {
            ImageFile *imgf = static_cast<ImageFile*>(obj);
            inspWidget = new ImageInspectorWidget(*imgf);
        }
        else if ( Object::IsOfType<MaterialFile>(obj) )
        {
            MaterialFile *matF = static_cast<MaterialFile*>(obj);
            inspWidget = new MaterialInspectorWidget(*matF );
        }
        else
        {
            inspWidget = new InspectorWidget();
        }
    }

    inspWidget->Init();
    inspWidget->Refresh();
    return inspWidget;
}
