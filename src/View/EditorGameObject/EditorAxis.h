#ifndef EDITORAXIS_H
#define EDITORAXIS_H

#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/EditorGizmo.h"

class Input;
class Camera;
class Renderer;
class Material;
class Transform;
class GameObject;
class EditorAxis : public EditorGizmo
{
public:
    enum EditorAxisDirection { X, Y, Z, Central };

protected:

    Material *m_material = nullptr;

    /**
     * @brief Line color will be multiplied by this when axis not being hovered.
     */
    const float c_colorFadingNotHover = 0.7f;
    /**
     * @brief Line color will be multiplied by this when axis being hovered,
     * but not grabbed.
     */
    const float c_colorFadingNotGrabbed = 1.0f;
    const Color c_colorGrabbed = Color(1.0f, 1.0f, 0.0f, 1.0f);
    Color m_lineColor = Color::Black;

    /**
     * @brief Axis direction in obj space.
     */
    Vector3 m_oAxisDirection;

    float m_axisLineWidth = 2.0f;

    static String GetStringFromDir(EditorAxisDirection dir);
    static Vector3 GetVectorFromDir(EditorAxisDirection dir);


    /**
     * @brief To be filled by child classes
    **/
    virtual Renderer *GetAxisRenderer() const = 0;

public:
    EditorAxis(EditorAxisDirection dir, GameObject *m_attachedGameObject);
    virtual ~EditorAxis();

    virtual void OnUpdate() override;
};

#endif // EDITORAXIS_H
