#include "EditorStage.h"

EditorStage::EditorStage() : Stage()
{
    cameraEntity = new EditorCamera();
    AddChild(cameraEntity);
    SetCameraChild(cameraEntity->GetName());
}

EditorStage::~EditorStage()
{
}
