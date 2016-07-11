#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"
#include "EditorFloor.h"
#include "EditorCamera.h"
#include "SelectionFramebuffer.h"
#include "EditorDebugGameObject.h"
#include "EditorTranslateAxisGroup.h"

class EditorScene : public Scene
{
private:
    EditorDebugGameObject *p_debugGameObject = nullptr;
    EditorCamera *p_cameraGameObject = nullptr;
    EditorTranslateAxisGroup *p_axises = nullptr;
    EditorFloor *p_floor = nullptr;

    SelectionFramebuffer *p_selectionFramebuffer = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();

    void _OnResize (int newWidth, int newHeight) override;
    void _OnRender () override;

    void DebugDrawLine(const Vector3 &origin,
                       const Vector3 &destiny,
                       float lineWidth = 1.0f,
                       float secsTime = 1.0f,
                       bool depthTest = true);

    virtual void DebugDrawScreenLine (
                                const glm::vec2 &origin,
                                const glm::vec2 &destiny,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true);

    bool IsEditorGameObject() const override;
};

#endif // EDITORSCENE_H
