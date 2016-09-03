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

    m_selectionFramebuffer =
            new SelectionFramebuffer(Screen::GetWidth(),
                                     Screen::GetHeight());
}

EditorScene::~EditorScene()
{
    Gizmos::SetGizmosGameObject(nullptr);
    Hierarchy::GetInstance()->Clear(); // To avoid potential bugs (seriously xd)
    delete m_selectionFramebuffer;
}

void EditorScene::_OnStart()
{
    Scene::_OnStart();
}

void EditorScene::_OnResize(int newWidth, int newHeight)
{
    Scene::_OnResize(newWidth, newHeight);
    m_selectionFramebuffer->Resize(newWidth, newHeight);
}

void EditorScene::_OnRender()
{
    Scene::_OnRender();

    m_selectionFramebuffer->Bind();
    m_selectionFramebuffer->Clear();
    m_selectionFramebuffer->RenderSelectionBuffer(this);
    m_selectionFramebuffer->UnBind();

    m_selectionFramebuffer->ProcessSelection();

    // Discomment to see selectionFramebuffer rendering
    // glClear(GL_DEPTH_BUFFER_BIT);
    // m_selectionFramebuffer->RenderSelectionBuffer(this);
}

SelectionFramebuffer* EditorScene::GetSelectionFramebuffer() const
{
    return m_selectionFramebuffer;
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
