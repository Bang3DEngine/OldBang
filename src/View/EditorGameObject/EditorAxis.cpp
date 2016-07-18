#include "EditorAxis.h"

#include "Canvas.h"

EditorAxis::EditorAxis(EditorAxis::EditorAxisDirection dir,
                       GameObject *attachedGameObject)
{
    this->p_attachedGameObject = attachedGameObject;

    m_oAxisDirection = EditorAxis::GetVectorFromDir(dir);

    p_material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/lines.bmat");
    p_material = new Material(*p_material);

    m_lineColor = m_oAxisDirection.Abs();

    this->SetRenderLayer(5);
}

EditorAxis::~EditorAxis()
{
    delete p_material;
}

std::string EditorAxis::GetStringFromDir(EditorAxis::EditorAxisDirection dir)
{
    if(dir == EditorAxisDirection::X) return "X";
    else if (dir == EditorAxisDirection::Y) return "Y";
    else return "Z";
}

Vector3 EditorAxis::GetVectorFromDir(EditorAxis::EditorAxisDirection dir)
{
    if(dir == EditorAxisDirection::X) return Vector3::right;
    else if (dir == EditorAxisDirection::Y) return Vector3::up;
    else return Vector3::forward;
}

void EditorAxis::OnUpdate()
{
    EditorGameObject::OnUpdate();

    // Process grabbing
    if(m_mouseIsOver && Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        // User has clicked on me!
        m_grabbed = true;
    }

    if(m_grabbed)
    {
        if(Input::GetMouseButtonUp(Input::MouseButton::MLeft))
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
            p_material->SetDiffuseColor(glm::vec4(m_lineColor, 1));
        }
        else
        {
            p_material->SetDiffuseColor(glm::vec4(m_lineColor * c_lineColorFadingNotGrabbed, 1));
        }
    }
    else
    {
        if (m_grabbed)
        {
            p_material->SetDiffuseColor(glm::vec4(m_lineColor, 1));
        }
        else
        {
            GetAxisRenderer()->SetLineWidth(m_axisLineWidth);
            p_material->SetDiffuseColor(glm::vec4(m_lineColor * c_lineColorFadingNotHover, 1));
        }
    }
}

void EditorAxis::OnMouseOver()
{
    EditorGameObject::OnMouseOver();
    m_mouseIsOver = true;
}

void EditorAxis::OnMouseExit()
{
    EditorGameObject::OnMouseExit();
    m_mouseIsOver = false;
}
