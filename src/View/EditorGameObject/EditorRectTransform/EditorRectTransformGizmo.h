#ifndef EDITORRECTTRANSFORMGIZMO_H
#define EDITORRECTTRANSFORMGIZMO_H

#include "EditorGameObject.h"

class EditorRectTransformCornerGizmo;
class EditorRectTransformGizmo : public EditorGameObject
{
public:
    EditorRectTransformGizmo(GameObject *attachedGameObject);
    virtual ~EditorRectTransformGizmo();

    void OnUpdate() override;
    void OnDrawGizmos() override;

private:
    EditorRectTransformCornerGizmo *m_leftTopCorner  = nullptr;
    EditorRectTransformCornerGizmo *m_rightTopCorner = nullptr;
    EditorRectTransformCornerGizmo *m_leftBotCorner  = nullptr;
    EditorRectTransformCornerGizmo *m_rightBotCorner = nullptr;
    GameObject *m_attachedGameObject = nullptr;
};

#endif // EDITORRECTTRANSFORMGIZMO_H
