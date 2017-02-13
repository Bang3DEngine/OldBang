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

    if (m_mouseIsOver &&
        Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        m_grabbed = true;
        Cursor::SetIcon(m_cursorIconWhenGrabbed);
    }

    if (m_grabbed)
    {
        if (Input::GetMouseButtonUp(Input::MouseButton::MLeft))
        {
            m_grabbed = false;
            Input::SetMouseWrapping(false);
            Cursor::SetIcon(m_mouseIsOver ? m_cursorIconWhenOver
                                          : Cursor::CursorIcon::Arrow);
        }
        else
        {
            Input::SetMouseWrapping(true);
            Cursor::SetIcon(m_cursorIconWhenGrabbed);
        }
    }
}

void EditorGizmo::OnMouseEnter(bool fromChildren)
{
    EditorGameObject::OnMouseEnter(fromChildren);
    if (!m_mouseIsOver)
    {
        m_mouseIsOver = true;
        if (!m_grabbed)
        {
            Cursor::SetIcon(m_cursorIconWhenOver);
        }
    }
}

void EditorGizmo::OnMouseExit(bool fromChildren)
{
    EditorGameObject::OnMouseExit(fromChildren);
    if (m_mouseIsOver)
    {
        m_mouseIsOver = false;
        if (!m_grabbed)
        {
            Cursor::SetIcon(Cursor::CursorIcon::Arrow);
        }
    }
}

