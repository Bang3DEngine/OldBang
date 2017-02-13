#ifndef EDITORGIZMO_H
#define EDITORGIZMO_H

#include "EditorGameObject.h"

class EditorGizmo : public EditorGameObject
{
public:
    EditorGizmo(GameObject *attachedGameObject);
    virtual ~EditorGizmo();

    void OnMouseEnter(bool fromChildren) override;
    void OnMouseExit(bool fromChildren) override;

protected:
    bool m_mouseIsOver = false;
    GameObject *m_attachedGameObject = nullptr;
};

#endif // EDITORGIZMO_H
