#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"
#include "EditorAxis.h"
#include "EditorCamera.h"

class EditorScene : public Scene
{
private:
    EditorCamera *cameraGameObject = nullptr;
    EditorAxis *axises = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();
};

#endif // EDITORSCENE_H
