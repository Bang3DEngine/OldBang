#ifndef EDITORAXIS_H
#define EDITORAXIS_H

#include "EditorGameObject.h"
#include "LineRenderer.h"
#include "Transform.h"
#include "Material.h"
#include "Matrix4.h"

class EditorTranslateAxis : public EditorGameObject
{
private:
    Transform *transform = nullptr;
    LineRenderer *line = nullptr;
    Material *material = nullptr;

    /**
     * @brief To know when the user is grabbing the axis with the mouse
     */
    bool grabbed = false;

    /**
     * @brief Line color will be multiplied by this when axis not being hovered.
     */
    Vector3 lineColorFadingNotHover = Vector3(0.5f);
    /**
     * @brief Line color will be multiplied by this when axis being hovered, but not grabbed.
     */
    Vector3 lineColorFadingNotGrabbed = Vector3(0.75f);
    Vector3 lineColor;

    Vector3 axisDirection;

    bool mouseIsOver = false;

public:
    enum EditorTranslateAxisDirection { X, Y, Z };
    EditorTranslateAxis(EditorTranslateAxisDirection dir);

    void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORAXIS_H
