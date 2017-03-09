#ifndef EDITORRECTTRANSFORMCORNERGIZMO_H
#define EDITORRECTTRANSFORMCORNERGIZMO_H

#include "Texture2D.h"
#include "EditorGizmo.h"

class RectTransform;
class EditorRectTransformGizmo;
class EditorRectTransformCornerGizmo : public EditorGizmo
{
public:
    enum CornerPosition
    {
        TopLeft,
        TopRight,
        BotRight,
        BotLeft,
        Center // Center corner... hehe
    };

    EditorRectTransformCornerGizmo(CornerPosition cornerPosition,
                                   GameObject *attachedGameObject);
    virtual ~EditorRectTransformCornerGizmo();

    void OnUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;

private:
    EditorRectTransformGizmo *m_parentGizmo = nullptr;
    RectTransform *m_attachedRectTransform = nullptr;
    Texture2D *m_circleTexture = nullptr;

    Color m_cornerColor = Color::Zero;
    CornerPosition m_cornerPosition = CornerPosition::TopLeft;

    Vector2 GetCornerOffset() const;
    void ApplyMarginDisplacement(const Vector2 &mouseDisp);
};

#endif // EDITORRECTTRANSFORMCORNERGIZMO_H
