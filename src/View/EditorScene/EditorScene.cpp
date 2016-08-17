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

    // Discomment to see selectionFramebuffer rendering
    //glDisable(GL_DEPTH_TEST);
    //m_selectionFramebuffer->RenderSelectionBuffer(this);
    //glEnable(GL_DEPTH_TEST);
}

SelectionFramebuffer* EditorScene::GetSelectionFramebuffer() const
{
    return m_selectionFramebuffer;
}

void EditorScene::DebugDrawLine(const Vector3 &origin, const Vector3 &destiny, const Color &color,
                                float lineWidth, float secsTime, bool depthTest)
{
    m_debugGameObject->DrawLine(origin, destiny, color, lineWidth, secsTime, depthTest);
}

void EditorScene::DebugDrawScreenLine(const Vector2 &origin, const Vector2 &destiny, const Color &color,
                                      float lineWidth, float secsTime)
{
    m_debugGameObject->DrawLineScreen(origin, destiny, color, lineWidth, secsTime);
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
