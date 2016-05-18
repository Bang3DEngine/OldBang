#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    debugGameObject = new EditorDebugGameObject();
    AddChild(debugGameObject);

    cameraGameObject = new EditorCamera();
    AddChild(cameraGameObject);
    SetCamera(cameraGameObject->GetCamera());

    axises = new EditorAxis();
    AddChild(axises);
}

EditorScene::~EditorScene()
{
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
