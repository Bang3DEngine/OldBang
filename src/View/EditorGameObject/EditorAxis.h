#ifndef EDITORAXIS_H
#define EDITORAXIS_H

#include "Bang.h"

#include "Input.h"
#include "Camera.h"
#include "Renderer.h"
#include "Transform.h"
#include "EditorGameObject.h"

class EditorAxis : public EditorGameObject
{
public:
    enum EditorAxisDirection { X, Y, Z };

private:
    Vector3 GetVectorFromDir(EditorAxisDirection dir) const;

protected:
    void GetMatrices(Matrix4 &pvm, Matrix4 &pv,
                     Matrix4 &p, Matrix4 &v, Matrix4 &m) const;

    Material *material = nullptr;

    /**
     * @brief To know when the user is grabbing the axis with the mouse
     */
    bool grabbed = false;

    /**
     * @brief Line color will be multiplied by this when axis not being hovered.
     */
    Vector3 lineColorFadingNotHover = Vector3(0.7f);
    /**
     * @brief Line color will be multiplied by this when axis being hovered, but not grabbed.
     */
    Vector3 lineColorFadingNotGrabbed = Vector3(0.9f);
    Vector3 lineColor;

    /**
     * @brief Axis direction in obj space.
     */
    Vector3 oAxisDirection;

    bool mouseIsOver = false;

    float lineWidth = 3.0f;

    /**
     * @brief To be filled by child classes
    **/
    virtual Renderer *GetAxisRenderer() const = 0;

public:
    EditorAxis(EditorAxisDirection dir);
    virtual ~EditorAxis();

    virtual void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORAXIS_H
