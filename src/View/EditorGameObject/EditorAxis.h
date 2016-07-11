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

    Material *p_material = nullptr;
    GameObject *p_attachedGameObject = nullptr;

    /**
     * @brief To know when the user is grabbing the axis with the mouse
     */
    bool m_grabbed = false;

    /**
     * @brief Line color will be multiplied by this when axis not being hovered.
     */
    const Vector3 c_lineColorFadingNotHover = Vector3(0.7f);
    /**
     * @brief Line color will be multiplied by this when axis being hovered, but not grabbed.
     */
    const Vector3 c_lineColorFadingNotGrabbed = Vector3(0.9f);
    Vector3 m_lineColor;

    /**
     * @brief Axis direction in obj space.
     */
    Vector3 m_oAxisDirection;

    bool m_mouseIsOver = false;

    float m_axisLineWidth = 1.0f;

    static std::string GetStringFromDir(EditorAxisDirection dir);
    static Vector3 GetVectorFromDir(EditorAxisDirection dir);


    /**
     * @brief To be filled by child classes
    **/
    virtual Renderer *GetAxisRenderer() const = 0;

public:
    EditorAxis(EditorAxisDirection dir, GameObject *p_attachedGameObject);
    virtual ~EditorAxis();

    virtual void OnUpdate() override;

    void OnMouseEnter() override;
    void OnMouseExit() override;
};

#endif // EDITORAXIS_H
