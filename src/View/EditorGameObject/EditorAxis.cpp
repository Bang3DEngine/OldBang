#include "EditorAxis.h"

#include "Input.h"
#include "Camera.h"
#include "Screen.h"
#include "Toolbar.h"
#include "Material.h"
#include "Renderer.h"
#include "Transform.h"
#include "AssetsManager.h"

EditorAxis::EditorAxis(EditorAxis::EditorAxisDirection dir,
                       GameObject *attachedGameObject)
{
    m_attachedGameObject = attachedGameObject;

    m_oAxisDirection = EditorAxis::GetVectorFromDir(dir);

    m_material = AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true);
    m_material = new Material(*m_material);

    m_lineColor = Color(m_oAxisDirection.Abs(), 1);
}

EditorAxis::~EditorAxis()
{
    delete m_material;
}

String EditorAxis::GetStringFromDir(EditorAxis::EditorAxisDirection dir)
{
    if (dir == EditorAxisDirection::X) return "X";
    else if (dir == EditorAxisDirection::Y) return "Y";
    else if (dir == EditorAxisDirection::Z) return "Z";
    else return "Central";
}

Vector3 EditorAxis::GetVectorFromDir(EditorAxis::EditorAxisDirection dir)
{
    if (dir == EditorAxisDirection::X) return Vector3::Right;
    else if (dir == EditorAxisDirection::Y) return Vector3::Up;
    else if (dir == EditorAxisDirection::Z) return Vector3::Forward;
    else return Vector3::One; // Central
}

void EditorAxis::OnUpdate()
{
    EditorGameObject::OnUpdate();

    // Process grabbing
    if (m_mouseIsOver && Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        // User has clicked on me!
        m_grabbed = true;
    }

    if (m_grabbed)
    {
        if (Input::GetMouseButtonUp(Input::MouseButton::MLeft))
        {
            m_grabbed = false;
            Input::SetMouseWrapping(false);
        }
        else
        {
            Input::SetMouseWrapping(true);
        }
    }

    // Process line color
    bool thickerLine = m_mouseIsOver || m_grabbed;
    float lineWidth = m_axisLineWidth * (thickerLine ? 1.5f : 1.0f);

    Color diffColor;
    if (m_grabbed)
    {
        diffColor = c_colorGrabbed;
    }
    else
    {
        float fading = m_mouseIsOver ? c_colorFadingNotGrabbed :
                                       c_colorFadingNotHover;
        diffColor = Color(m_lineColor * fading, 1);
    }

    GetAxisRenderer()->SetLineWidth(lineWidth);
    m_material->SetDiffuseColor(diffColor);
}

void EditorAxis::OnMouseEnter(bool fromChildren)
{
    EditorGameObject::OnMouseEnter(fromChildren);
    m_mouseIsOver = true;
}

void EditorAxis::OnMouseExit(bool fromChildren)
{
    EditorGameObject::OnMouseExit(fromChildren);
    m_mouseIsOver = false;
}
