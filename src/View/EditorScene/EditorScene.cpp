#include "EditorScene.h"

#include "Hierarchy.h"

EditorScene::EditorScene() : Scene()
{
    EditorGameObject *gizmosGo = new EditorGameObject("BANG_GizmosGameObject");
    gizmosGo->SetParent(this);
    Gizmos::SetGizmosGameObject(gizmosGo);

    m_debugGameObject = new EditorDebugGameObject();
    m_debugGameObject->SetParent(this);

    m_cameraGameObject = new EditorCamera();
    m_cameraGameObject->SetParent(this);
    SetCamera(m_cameraGameObject->GetCamera());

    //axises = new EditorTranslateAxisGroup();
    //axises->SetParent(this);

    m_floor = new EditorFloor();
    m_floor->SetParent(this);
}

EditorScene::~EditorScene()
{
    Gizmos::SetGizmosGameObject(nullptr);
    Hierarchy::GetInstance()->Clear(); // To avoid potential bugs (seriously xd)
}

void EditorScene::_OnStart()
{
    Scene::_OnStart();
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
