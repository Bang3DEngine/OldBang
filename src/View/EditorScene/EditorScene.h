#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"
#include "EditorCamera.h"
#include "EditorAxisGroup.h"
#include "SelectionFramebuffer.h"
#include "EditorDebugGameObject.h"

class EditorScene : public Scene
{
private:
    EditorDebugGameObject *debugGameObject = nullptr;
    EditorCamera *cameraGameObject = nullptr;
    EditorTranslateAxisGroup *axises = nullptr;

    SelectionFramebuffer *selectionFramebuffer = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();

    void _OnResize (int newWidth, int newHeight) override;
    void _OnRender () override;

    void DebugDrawLine(const Vector3 &origin,
                       const Vector3 &destiny,
                       float lineWidth = 1.0f,
                       int msTime = -1,
                       bool depthTest = true) override;

    bool IsEditorGameObject() const override;
};

#endif // EDITORSCENE_H
