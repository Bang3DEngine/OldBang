#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    p_debugGameObject = new EditorDebugGameObject();
    AddChild(p_debugGameObject);

    p_cameraGameObject = new EditorCamera();
    AddChild(p_cameraGameObject);
    SetCamera(p_cameraGameObject->GetCamera());

    //axises = new EditorTranslateAxisGroup();
    //AddChild(axises);

    p_floor = new EditorFloor();
    AddChild(p_floor);

    p_selectionFramebuffer =
            new SelectionFramebuffer(Canvas::GetWidth(),
                                     Canvas::GetHeight());
}

EditorScene::~EditorScene()
{
    delete p_selectionFramebuffer;
}

void EditorScene::_OnResize(int newWidth, int newHeight)
{
    Scene::_OnResize(newWidth, newHeight);
    p_selectionFramebuffer->Resize(newWidth, newHeight);
}

void EditorScene::_OnRender()
{
    Scene::_OnRender();

    //From 0 to 9
    p_selectionFramebuffer->Bind();
    p_selectionFramebuffer->Clear();
    for(m_currentRenderLayer = 0; m_currentRenderLayer <= 9; ++m_currentRenderLayer)
    {
        p_selectionFramebuffer->ClearDepth();
        p_selectionFramebuffer->RenderSelectionBuffer(this);
    }
    p_selectionFramebuffer->UnBind();

    p_selectionFramebuffer->ProcessSelection();

    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(m_currentRenderLayer = 0; m_currentRenderLayer <= 9; ++m_currentRenderLayer)
    {
        p_selectionFramebuffer->ClearDepth();
        p_selectionFramebuffer->RenderSelectionBuffer(this);
    }
    */
}

void EditorScene::DebugDrawLine(const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth,
                                float secsTime,
                                bool depthTest)
{
    p_debugGameObject->DrawLine(origin, destiny, lineWidth, secsTime, depthTest);
}

void EditorScene::DebugDrawScreenLine(const glm::vec2 &origin, const glm::vec2 &destiny,
                                      float lineWidth, float secsTime, bool depthTest)
{
    p_debugGameObject->DrawLineScreen(origin, destiny, lineWidth, secsTime, depthTest);
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
