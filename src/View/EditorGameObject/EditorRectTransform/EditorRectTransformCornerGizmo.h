#ifndef EDITORRECTTRANSFORMCORNERGIZMO_H
#define EDITORRECTTRANSFORMCORNERGIZMO_H

#include "Texture2D.h"
#include "EditorGizmo.h"

class EditorRectTransformCornerGizmo : public EditorGizmo
{
public:
    enum CornerPosition
    {
        TopLeft,
        TopRight,
        BotRight,
        BotLeft
    };

    EditorRectTransformCornerGizmo(CornerPosition cornerPosition,
                                   GameObject *attachedGameObject);
    virtual ~EditorRectTransformCornerGizmo();

    void OnUpdate() override;
    void OnDrawGizmosOverlay() override;

private:
    Color m_cornerColor = Color::Zero;
    Texture2D *m_circleTexture = nullptr;
    CornerPosition m_cornerPosition = CornerPosition::TopLeft;

    Vector2 GetCornerOffset() const;
};

#endif // EDITORRECTTRANSFORMCORNERGIZMO_H
