#ifndef EDITORROTATEAXIS_H
#define EDITORROTATEAXIS_H

#include "Transform.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"

class EditorRotateAxis : public EditorGameObject
{
private:

    const float rotationBoost = 10.0f;

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

    /**
     * @brief These anchorIndexes refer to indexes of the Circle rendered by the CircleRenderer.
     * Specifically, they are the indexes of the 2 closest circle points from the mouse position of
     * the last time user clicked the mouse in the axis. In other words, the indexes refer to the 2
     * closest points in the circle from the mouse, when the user last clicked on top of the rotationAxis.
     * (Distances area measured in screen space)
     * We will use these as a frame of reference to know which sense it should rotate, which direction the
     * mouse has to be aligned to in order to get the maximum alignment ratio (1.0), etc.
     * We don't update these anchorIndexes every frame, because it is weird to rotate the circle like that.
     * We just update them every time the user wants to rotate, and keep them during all rotation (until the user
     * releases its mouse button)
     *
     * In addition anchorIndex0 < anchorIndex1. This helps us to define the sense of rotation.
     * In fact, the 2 keys for this to work, is to compute stuff and the alignment in screen space,
     * but without forgetting the original indexes that the anchor points had in the circle.
     */
    int anchorIndex0, anchorIndex1;

    /**
     * @brief Points of the circle corresponding to anchorIndex0, anchorIndex1.
     * In Screen Space.
     */
    glm::vec2 sAnchorPoint0, sAnchorPoint1;

    bool mouseIsOver = false;

public:
    enum EditorRotateAxisDirection { X, Y, Z };
    EditorRotateAxis(EditorRotateAxisDirection dir);

    void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORROTATEAXIS_H
