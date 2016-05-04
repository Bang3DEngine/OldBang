#include "EditorScene.h"

EditorScene::EditorScene() : Scene()
{
    cameraGameObject = new EditorCamera();
    AddChild(cameraGameObject);
    SetCameraChild(cameraGameObject->GetName());

    axises = new EditorAxis();
    AddChild(axises);
}

EditorScene::~EditorScene()
{
}
