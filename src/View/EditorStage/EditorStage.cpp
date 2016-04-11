#include "EditorStage.h"

EditorStage::EditorStage() : Stage()
{
    editorCamera = new EditorCamera();
    AddChild(editorCamera);
    SetCameraChild(editorCamera->GetName());
}

EditorStage::~EditorStage()
{
    delete editorCamera;
}
