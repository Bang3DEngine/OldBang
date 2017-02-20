#ifndef EDITORGIZMO_H
#define EDITORGIZMO_H

#include "Cursor.h"
#include "GameObject.h"

class EditorGizmo : public GameObject
{
public:
    EditorGizmo(GameObject *attachedGameObject);
    virtual ~EditorGizmo();

    virtual void OnUpdate() override;
    virtual void OnMouseEnter(bool fromChildren) override;
    virtual void OnMouseExit(bool fromChildren) override;

protected:
    Cursor::CursorIcon m_cursorIconWhenOver    = Cursor::CursorIcon::Arrow;
    Cursor::CursorIcon m_cursorIconWhenGrabbed = Cursor::CursorIcon::Arrow;

    bool m_grabbed = false;
    bool m_mouseIsOver = false;
    GameObject *m_attachedGameObject = nullptr;
};

#endif // EDITORGIZMO_H
