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

protected:

    Material *material = nullptr;
    GameObject *attachedGameObject = nullptr;

    static std::string GetStringFromDir(EditorAxisDirection dir);
    static Vector3 GetVectorFromDir(EditorAxisDirection dir);

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

    float axisLineWidth = 1.0f;

    /**
     * @brief To be filled by child classes
    **/
    virtual Renderer *GetAxisRenderer() const = 0;

public:
    EditorAxis(EditorAxisDirection dir, GameObject *attachedGameObject);
    virtual ~EditorAxis();

    virtual void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORAXIS_H
