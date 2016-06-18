#ifndef EDITORROTATEAXIS_H
#define EDITORROTATEAXIS_H

#include "Transform.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"

class EditorRotateAxis : public EditorGameObject
{
private:
    Transform *transform = nullptr;
    CircleRenderer *circle = nullptr;
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
    enum EditorRotateAxisDirection { X, Y, Z };
    EditorRotateAxis(EditorRotateAxisDirection dir);

    void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORROTATEAXIS_H
