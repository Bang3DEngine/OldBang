#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    debugGameObject = new EditorDebugGameObject();
    AddChild(debugGameObject);

    cameraGameObject = new EditorCamera();
    AddChild(cameraGameObject);
    SetCamera(cameraGameObject->GetCamera());

    axises = new EditorAxisGroup();
    //AddChild(axises);

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
void EditorScene::_OnRender(unsigned char _renderLayer)
{
    Scene::_OnRender(_renderLayer);

    //selectionFramebuffer->Bind();
    selectionFramebuffer->RenderSelectionBuffer(this);
    selectionFramebuffer->ProcessSelection();
    //selectionFramebuffer->UnBind();
}

void EditorScene::DebugDrawLine(const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth,
                                int msTime,
                                bool depthTest)
{
    debugGameObject->DrawLine(origin, destiny, lineWidth, msTime, depthTest);
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
