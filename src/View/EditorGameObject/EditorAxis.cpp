#include "EditorAxis.h"

#include "Canvas.h"
#include "Toolbar.h"

EditorAxis::EditorAxis(EditorAxis::EditorAxisDirection dir,
                       GameObject *attachedGameObject)
{
    this->m_attachedGameObject = attachedGameObject;

    m_oAxisDirection = EditorAxis::GetVectorFromDir(dir);

    m_material = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/D2G_Line.bmat");
    m_material = new Material(*m_material);

    m_lineColor = m_oAxisDirection.Abs();
}

EditorAxis::~EditorAxis()
{
    delete m_material;
}

std::string EditorAxis::GetStringFromDir(EditorAxis::EditorAxisDirection dir)
{
    if (dir == EditorAxisDirection::X) return "X";
    else if (dir == EditorAxisDirection::Y) return "Y";
    else return "Z";
}

Vector3 EditorAxis::GetVectorFromDir(EditorAxis::EditorAxisDirection dir)
{
    if (dir == EditorAxisDirection::X) return Vector3::right;
    else if (dir == EditorAxisDirection::Y) return Vector3::up;
    else return Vector3::forward;
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
    if (m_mouseIsOver)
    {
        GetAxisRenderer()->SetLineWidth(m_axisLineWidth * 2.0f);

        if (m_grabbed)
        {
            m_material->SetDiffuseColor(glm::vec4(m_lineColor, 1));
        }
        else
        {
            m_material->SetDiffuseColor(glm::vec4(m_lineColor * c_lineColorFadingNotGrabbed, 1));
        }
    }
    else
    {
        if (m_grabbed)
        {
            m_material->SetDiffuseColor(glm::vec4(m_lineColor, 1));
        }
        else
        {
            GetAxisRenderer()->SetLineWidth(m_axisLineWidth);
            m_material->SetDiffuseColor(glm::vec4(m_lineColor * c_lineColorFadingNotHover, 1));
        }
    }
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
