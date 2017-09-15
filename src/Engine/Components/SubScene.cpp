#include "Bang/SubScene.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"

USING_NAMESPACE_BANG

SubScene::SubScene()
{

}

SubScene::~SubScene()
{

}

void SubScene::OnBeforeChildrenRender(RenderPass renderPass)
{
    Scene *scene = DCAST<Scene*>(gameObject);
    if (!scene)
    {
        Debug_Warn("A " << GetClassName() << " must only be attached to a Scene.");
        return;
    }

    Scene *parentScene = GetParentScene(); ENSURE(parentScene);
    if (!parentScene)
    {
        Debug_Warn("A " << GetClassName() << " must be attached to a Scene that"
                   " is a child of another Scene");
        return;
    }

    p_previousCamera = parentScene->GetCamera();

    Camera *newCamera = scene->GetCamera();
    parentScene->SetCamera(newCamera);
    if (newCamera) { newCamera->Bind(); }
}

void SubScene::OnChildrenRendered(RenderPass renderPass)
{
    Scene *scene = DCAST<Scene*>(gameObject); ENSURE(scene);
    Scene *parentScene = GetParentScene(); ENSURE(parentScene);

    parentScene->SetCamera(p_previousCamera);
}

Scene *SubScene::GetParentScene() const
{
    GameObject *go = gameObject;
    while (go != nullptr)
    {
        Scene *sc = DCAST<Scene*>(go);
        if (sc) { return sc; }
        else { go = go->parent; }
    }
    return nullptr;
}
