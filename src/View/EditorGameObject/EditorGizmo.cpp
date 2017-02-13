#include "EditorGizmo.h"

#include "Input.h"

EditorGizmo::EditorGizmo(GameObject *attachedGameObject)
{
    m_attachedGameObject = attachedGameObject;
}

EditorGizmo::~EditorGizmo()
{

}

void EditorGizmo::OnUpdate()
{
    EditorGameObject::OnUpdate();

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
}

void EditorGizmo::OnMouseEnter(bool fromChildren)
{
    EditorGameObject::OnMouseEnter(fromChildren);
    m_mouseIsOver = true;
}

void EditorGizmo::OnMouseExit(bool fromChildren)
{
    EditorGameObject::OnMouseExit(fromChildren);
    m_mouseIsOver = false;
}

