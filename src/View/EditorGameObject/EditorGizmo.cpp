#include "EditorGizmo.h"

EditorGizmo::EditorGizmo(GameObject *attachedGameObject)
{
    m_attachedGameObject = attachedGameObject;
}

EditorGizmo::~EditorGizmo()
{

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

