#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"
#include "EditorAxis.h"
#include "EditorCamera.h"
#include "EditorDebugGameObject.h"

class EditorScene : public Scene
{
private:
    EditorDebugGameObject *debugGameObject = nullptr;
    EditorCamera *cameraGameObject = nullptr;
    EditorAxis *axises = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();

    void DebugDrawLine(const Vector3 &origin,
                       const Vector3 &destiny,
                       float lineWidth = 1.0f,
                       int msTime = -1,
                       bool depthTest = true) override;

    bool IsEditorGameObject() const override;
};

#endif // EDITORSCENE_H
