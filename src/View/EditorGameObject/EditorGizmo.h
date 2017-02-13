#ifndef EDITORGIZMO_H
#define EDITORGIZMO_H

#include "EditorGameObject.h"

class EditorGizmo : public EditorGameObject
{
public:
    EditorGizmo(GameObject *attachedGameObject);
    virtual ~EditorGizmo();

    virtual void OnUpdate() override;
    virtual void OnMouseEnter(bool fromChildren) override;
    virtual void OnMouseExit(bool fromChildren) override;

protected:
    bool m_grabbed = false;
    bool m_mouseIsOver = false;
    GameObject *m_attachedGameObject = nullptr;
};

#endif // EDITORGIZMO_H
