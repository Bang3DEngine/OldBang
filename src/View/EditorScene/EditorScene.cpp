#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
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
            new SelectionFramebuffer(Canvas::GetWidth(),
                                     Canvas::GetHeight());
}

EditorScene::~EditorScene()
{
    delete m_selectionFramebuffer;
}

void EditorScene::_OnStart()
{
    Scene::_OnStart();

    EditorGameObject *gizmosGo = new EditorGameObject("BANG_GizmosGameObject");
    gizmosGo->SetParent(this);
    Gizmos::SetGizmosGameObject(gizmosGo);
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

    /* Discomment to see selectionFramebuffer rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_selectionFramebuffer->RenderSelectionBuffer(this);
    */
}

SelectionFramebuffer* EditorScene::GetSelectionFramebuffer() const
{
    return m_selectionFramebuffer;
}

void EditorScene::DebugDrawLine(const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth,
                                float secsTime,
                                bool depthTest)
{
    m_debugGameObject->DrawLine(origin, destiny, lineWidth, secsTime, depthTest);
}

void EditorScene::DebugDrawScreenLine(const glm::vec2 &origin, const glm::vec2 &destiny,
                                      float lineWidth, float secsTime, bool depthTest)
{
    m_debugGameObject->DrawLineScreen(origin, destiny, lineWidth, secsTime, depthTest);
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
