#ifndef EDITORROTATEAXIS_H
#define EDITORROTATEAXIS_H

#include "Transform.h"
#include "EditorAxis.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"

class EditorRotateAxis : public EditorAxis
{
private:

    const float rotationBoost = 0.1f;

    CircleRenderer *circle = nullptr;

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

    /**
     * @brief When working with local coords system activated, we must save the initial axis which
     * the user started to rotate with. Otherwise, it will do unexpected things for the user.
     */
    Vector3 currentOAxisDirection;

protected:
    Renderer *GetAxisRenderer() const override;

public:
    EditorRotateAxis(EditorAxis::EditorAxisDirection dir, GameObject *attachedGameObject);
    virtual ~EditorRotateAxis();

    void OnUpdate() override;
};

#endif // EDITORROTATEAXIS_H
