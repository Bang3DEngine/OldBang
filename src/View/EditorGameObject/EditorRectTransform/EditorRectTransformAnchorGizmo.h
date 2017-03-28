#ifndef EDITORRECTTRANSFORMANCHORGIZMO_H
#define EDITORRECTTRANSFORMANCHORGIZMO_H

#include "Bang/Vector2.h"
#include "Bang/EditorGizmo.h"

class Texture2D;
class RectTransform;
class EditorRectTransformAnchorGizmo : public EditorGizmo
{
public:
    enum AnchorPosition
    {
        TopLeft,
        TopRight,
        BotRight,
        BotLeft
    };

    EditorRectTransformAnchorGizmo(AnchorPosition anchorPosition,
                                   GameObject *attachedGameObject);
    virtual ~EditorRectTransformAnchorGizmo();

    void OnUpdate() override;
    void OnDrawGizmos(bool depthed, bool overlay) override;

private:
    RectTransform *m_attachedRectTransform = nullptr;
    Texture2D *m_anchorTexture = nullptr;

    const Vector2 c_anchorSize = 0.05f * Vector2(1, 1.25);
    Color m_anchorColor = Color::Zero;
    AnchorPosition m_anchorPosition = AnchorPosition::TopLeft;

    Vector2 GetAnchorCenter() const;
    Quaternion GetAnchorRotation() const;
    void ApplyDisplacementToAnchor(const Vector2 &mouseDisp);
};

#endif // EDITORRECTTRANSFORMANCHORGIZMO_H
