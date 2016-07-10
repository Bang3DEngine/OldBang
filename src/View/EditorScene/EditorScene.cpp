#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    debugGameObject = new EditorDebugGameObject();
    AddChild(debugGameObject);

    cameraGameObject = new EditorCamera();
    AddChild(cameraGameObject);
    SetCamera(cameraGameObject->GetCamera());

    //axises = new EditorTranslateAxisGroup();
    //AddChild(axises);

    floor = new EditorFloor();
    AddChild(floor);

    selectionFramebuffer =
            new SelectionFramebuffer(Canvas::GetWidth(),
                                     Canvas::GetHeight());
}

EditorScene::~EditorScene()
{
    delete selectionFramebuffer;
}

void EditorScene::_OnResize(int newWidth, int newHeight)
{
    Scene::_OnResize(newWidth, newHeight);
    selectionFramebuffer->Resize(newWidth, newHeight);
}

void EditorScene::_OnRender()
{
    Scene::_OnRender();

    //From 0 to 9
    selectionFramebuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    selectionFramebuffer->Clear();
    for(currentRenderLayer = 0; currentRenderLayer <= 9; ++currentRenderLayer)
    {
        selectionFramebuffer->ClearDepth();
        selectionFramebuffer->RenderSelectionBuffer(this);
    }
    selectionFramebuffer->UnBind();

    selectionFramebuffer->ProcessSelection();
}

void EditorScene::DebugDrawLine(const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth,
                                float secsTime,
                                bool depthTest)
{
    debugGameObject->DrawLine(origin, destiny, lineWidth, secsTime, depthTest);
}

void EditorScene::DebugDrawScreenLine(const glm::vec2 &origin, const glm::vec2 &destiny,
                                      float lineWidth, float secsTime, bool depthTest)
{
    debugGameObject->DrawLineScreen(origin, destiny, lineWidth, secsTime, depthTest);
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
