#ifndef GUISCROLLAREA_H
#define GUISCROLLAREA_H

#include "Bang/Vector2.h"
#include "Bang/GUIMask.h"
#include "Bang/UIGameObject.h"

class GUIScrollArea : public UIGameObject
{
    UIGAMEOBJECT(GUIScrollArea)

public:
    GUIScrollArea() noexcept;
    virtual ~GUIScrollArea() noexcept;

    void AddChild(UIGameObject *child) noexcept;

    void SetMasking(bool masking);
    void SetScrolling(const Vector2i &scrollPx) noexcept;
    void SetScrollingX(int scrollPxX) noexcept;
    void SetScrollingY(int scrollPxY) noexcept;

    bool GetMasking() const;
    const Vector2i& GetScrolling() const noexcept;

private:
    GUIMask *m_mask = nullptr;
    UIGameObject *m_childrenContainer = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void RefreshChildren();
};

#endif // GUISCROLLAREA_H
