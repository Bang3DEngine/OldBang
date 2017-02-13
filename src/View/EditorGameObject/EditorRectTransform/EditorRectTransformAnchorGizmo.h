#ifndef EDITORRECTTRANSFORMANCHORGIZMO_H
#define EDITORRECTTRANSFORMANCHORGIZMO_H

#include "EditorGizmo.h"

class Texture2D;
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
    void OnDrawGizmosOverlay() override;

private:
    const float c_anchorSize = 0.05f;
    Color m_anchorColor = Color::Zero;
    Texture2D *m_anchorTexture = nullptr;
    AnchorPosition m_anchorPosition = AnchorPosition::TopLeft;

    Vector2 GetAnchorCenter() const;
    Quaternion GetAnchorRotation() const;
};

#endif // EDITORRECTTRANSFORMANCHORGIZMO_H
