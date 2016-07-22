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

void EditorScene::_OnResize(int newWidth, int newHeight)
{
    Scene::_OnResize(newWidth, newHeight);
    m_selectionFramebuffer->Resize(newWidth, newHeight);
}

void EditorScene::_OnRender()
{
    Scene::_OnRender();

    //From 0 to 9
    m_selectionFramebuffer->Bind();
    m_selectionFramebuffer->Clear();
    for (m_currentRenderLayer = 0; m_currentRenderLayer <= 9; ++m_currentRenderLayer)
    {
        m_selectionFramebuffer->ClearDepth();
        m_selectionFramebuffer->RenderSelectionBuffer(this);
    }
    m_selectionFramebuffer->UnBind();

    m_selectionFramebuffer->ProcessSelection();

    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (m_currentRenderLayer = 0; m_currentRenderLayer <= 9; ++m_currentRenderLayer)
    {
        m_selectionFramebuffer->ClearDepth();
        m_selectionFramebuffer->RenderSelectionBuffer(this);
    }
    */
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
