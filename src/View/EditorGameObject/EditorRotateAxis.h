#ifndef EDITORROTATEAXIS_H
#define EDITORROTATEAXIS_H

#include "Transform.h"
#include "EditorAxis.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"

class EditorRotateAxis : public EditorAxis
{
friend class EditorRotateAxisGroup;

private:

    const float c_rotationBoost = 0.1f;

    CircleRenderer *m_circle = nullptr;

    /**
     * @brief These anchorPoints refer to points of the Circle rendered by the CircleRenderer.
     * Specifically, they are the points of the 2 closest circle points from the mouse position of
     * the last time user clicked the mouse in the axis. In other words, these points refer to the 2
     * closest points in the circle from the mouse, when the user last clicked on top of the rotationAxis.
     * We will use these as a frame of reference to know which sense it should rotate, which direction the
     * mouse has to be aligned to in order to get the maximum alignment ratio (1.0), etc.
     * We just update them every time the user wants to rotate, and keep them during all rotation (until the user
     * releases its mouse button)
     * In Screen Space.
     */
    Vector2 m_sAnchorPoint0, m_sAnchorPoint1;

protected:
    Renderer *GetAxisRenderer() const override;

public:
    EditorRotateAxis(EditorAxis::EditorAxisDirection dir, GameObject *m_attachedGameObject);
    virtual ~EditorRotateAxis();

    void OnUpdate() override;
    void OnDrawGizmosNoDepth() override;
};

#endif // EDITORROTATEAXIS_H
