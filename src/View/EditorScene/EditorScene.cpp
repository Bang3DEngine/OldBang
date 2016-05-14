#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    cameraGameObject = new EditorCamera();
    AddChild(cameraGameObject);
    SetCamera(cameraGameObject->GetCamera());

    axises = new EditorAxis();
    AddChild(axises);
}

EditorScene::~EditorScene()
{
}

bool EditorScene::IsEditorGameObject() const
{
    return true;
}
