#ifndef EDITORRECTTRANSFORMCORNERGIZMO_H
#define EDITORRECTTRANSFORMCORNERGIZMO_H

#include "Texture2D.h"
#include "EditorGameObject.h"

class EditorRectTransformCornerGizmo : public EditorGameObject
{
public:
    enum CornerPosition
    {
        TopLeft,
        TopRight,
        BotRight,
        BotLeft
    };

    EditorRectTransformCornerGizmo(GameObject *attachedGameObject,
                                   CornerPosition cornerPosition);
    virtual ~EditorRectTransformCornerGizmo();

    void OnUpdate() override;
    void OnDrawGizmos() override;

    void OnMouseEnter(bool fromChildren) override;
    void OnMouseExit(bool fromChildren) override;

private:
    Color m_cornerColor = Color::Zero;

    bool m_mouseIsOver = false;
    Texture2D *m_circleTexture = nullptr;
    GameObject *m_attachedGameObject = nullptr;
    CornerPosition m_cornerPosition = CornerPosition::TopLeft;

    Vector2 GetCornerOffset() const;
};

#endif // EDITORRECTTRANSFORMCORNERGIZMO_H
