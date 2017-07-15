#include "Bang/InspectorWidgetFactory.h"

#include "Bang/Debug.h"
#include "Bang/UIText.h"
#include "Bang/Camera.h"
#include "Bang/UIImage.h"
#include "Bang/FontFile.h"
#include "Bang/MeshFile.h"
#include "Bang/ModelFile.h"
#include "Bang/ImageFile.h"
#include "Bang/Transform.h"
#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/SoundFile.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/PrefabFile.h"
#include "Bang/AudioSource.h"
#include "Bang/MaterialFile.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Texture2DFile.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioClipFile.h"
#include "Bang/RectTransform.h"
#include "Bang/CircleRenderer.h"
#include "Bang/ComponentWidget.h"
#include "Bang/DirectionalLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/FontInspectorWidget.h"
#include "Bang/MeshInspectorWidget.h"
#include "Bang/InspectorWidgetGroup.h"
#include "Bang/ImageInspectorWidget.h"
#include "Bang/ModelInspectorWidget.h"
#include "Bang/SoundInspectorWidget.h"
#include "Bang/UITextInspectorWidget.h"
#include "Bang/CameraInspectorWidget.h"
#include "Bang/UIImageInspectorWidget.h"
#include "Bang/MaterialInspectorWidget.h"
#include "Bang/BehaviourInspectorWidget.h"
#include "Bang/Texture2DInspectorWidget.h"
#include "Bang/TransformInspectorWidget.h"
#include "Bang/AudioClipInspectorWidget.h"
#include "Bang/FileInspectorWidgetGroup.h"
#include "Bang/PointLightInspectorWidget.h"
#include "Bang/AssetInspectorWidgetGroup.h"
#include "Bang/AudioSourceInspectorWidget.h"
#include "Bang/PrefabInspectorWidgetGroup.h"
#include "Bang/SingleInspectorWidgetGroup.h"
#include "Bang/MeshRendererInspectorWidget.h"
#include "Bang/LineRendererInspectorWidget.h"
#include "Bang/RectTransformInspectorWidget.h"
#include "Bang/AudioListenerInspectorWidget.h"
#include "Bang/CircleRendererInspectorWidget.h"
#include "Bang/GameObjectInspectorWidgetGroup.h"
#include "Bang/DirectionalLightInspectorWidget.h"
#include "Bang/PostProcessEffectInspectorWidget.h"

InspectorWidgetGroup* InspectorWidgetFactory::CreateWidgetGroup(Object *obj)
{
    if (obj->IsOfType<GameObject>())
    {
        GameObject *go = Object::SCast<GameObject>(obj);
        return new GameObjectInspectorWidgetGroup(go);
    }

    if ( obj->IsOfType<AudioClipFile>() )
    {
        AudioClipFile *acf = static_cast<AudioClipFile*>(obj);
        return new AssetInspectorWidgetGroup<AudioClip>( new AudioClipInspectorWidget(*acf) );
    }

    if ( obj->IsOfType<FontFile>() )
    {
        FontFile *ff = static_cast<FontFile*>(obj);
        return new AssetInspectorWidgetGroup<Font>(new FontInspectorWidget(*ff) );
    }

    if ( obj->IsOfType<ImageFile>() )
    {
        ImageFile *imgf = static_cast<ImageFile*>(obj);
        return new FileInspectorWidgetGroup( new ImageInspectorWidget(*imgf) );
    }

    if ( obj->IsOfType<MaterialFile>() )
    {
        MaterialFile *matF = static_cast<MaterialFile*>(obj);
        return new AssetInspectorWidgetGroup<Material>( new MaterialInspectorWidget(*matF) );
    }

    if ( obj->IsOfType<MeshFile>() )
    {
        MeshFile *meshF = static_cast<MeshFile*>(obj);
        return new AssetInspectorWidgetGroup<Mesh>( new MeshInspectorWidget(*meshF) );
    }

    if ( obj->IsOfType<ModelFile>() )
    {
        ModelFile *modelF = static_cast<ModelFile*>(obj);
        return new FileInspectorWidgetGroup( new ModelInspectorWidget(*modelF) );
    }

    if ( obj->IsOfType<SoundFile>() )
    {
        SoundFile *soundF = static_cast<SoundFile*>(obj);
        return new FileInspectorWidgetGroup( new SoundInspectorWidget(*soundF) );
    }

    if ( obj->IsOfType<Texture2DFile>() )
    {
        Texture2DFile *texF = static_cast<Texture2DFile*>(obj);
        return new AssetInspectorWidgetGroup<Texture2D>(new Texture2DInspectorWidget(*texF) );
    }

    if ( obj->IsOfType<PrefabFile>() )
    {
        PrefabFile *prefF = static_cast<PrefabFile*>(obj);

        Prefab *prefab = AssetsManager::Load<Prefab>(prefF->GetPath());
        return new PrefabInspectorWidgetGroup(prefab);
    }

    BFile *f = static_cast<BFile*>(obj);
    return new FileInspectorWidgetGroup( new FileInspectorWidget(*f) );
}

InspectorWidget *InspectorWidgetFactory::CreateComponentWidget(Component *comp)
{
    if (comp->IsOfType<AudioSource>())
    {
        AudioSource *as = static_cast<AudioSource*>(comp);
        return new AudioSourceInspectorWidget(as);
    }

    if (comp->IsOfType<AudioListener>())
    {
        AudioListener *al = static_cast<AudioListener*>(comp);
        return new AudioListenerInspectorWidget(al);
    }

    if (comp->IsOfType<Behaviour>())
    {
        Behaviour *b = static_cast<Behaviour*>(comp);
        return new BehaviourInspectorWidget(b);
    }

    if (comp->IsOfType<Camera>())
    {
        Camera *c = static_cast<Camera*>(comp);
        return new CameraInspectorWidget(c);
    }

    if (comp->IsOfType<UIImage>())
    {
        UIImage *img = static_cast<UIImage*>(comp);
        return new UIImageInspectorWidget(img);
    }

    if (comp->IsOfType<UIText>())
    {
        UIText *txt = static_cast<UIText*>(comp);
        return new UITextInspectorWidget(txt);
    }

    if (comp->IsOfType<CircleRenderer>())
    {
        CircleRenderer *cr = static_cast<CircleRenderer*>(comp);
        return new CircleRendererInspectorWidget(cr);
    }

    if (comp->IsOfType<LineRenderer>())
    {
        LineRenderer *lr = static_cast<LineRenderer*>(comp);
        return new LineRendererInspectorWidget(lr);
    }

    if (comp->IsOfType<MeshRenderer>())
    {
        MeshRenderer *mr = static_cast<MeshRenderer*>(comp);
        return new MeshRendererInspectorWidget(mr);
    }

    if (comp->IsOfType<DirectionalLight>())
    {
        DirectionalLight *dl = static_cast<DirectionalLight*>(comp);
        return new DirectionalLightInspectorWidget(dl);
    }

    if (comp->IsOfType<PointLight>())
    {
        PointLight *pl = static_cast<PointLight*>(comp);
        return new PointLightInspectorWidget(pl);
    }

    if (comp->IsOfType<RectTransform>())
    {
        RectTransform *rt = static_cast<RectTransform*>(comp);
        return new RectTransformInspectorWidget(rt);
    }

    if (comp->IsOfType<Transform>())
    {
        Transform *tr = static_cast<Transform*>(comp);
        return new TransformInspectorWidget(tr);
    }

    if (comp->IsOfType<PostProcessEffect>())
    {
        PostProcessEffect *ppe = static_cast<PostProcessEffect*>(comp);
        return new PostProcessEffectInspectorWidget(ppe);
    }

    return new ComponentWidget( static_cast<Component*>(comp) );
}
