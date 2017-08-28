#ifndef GUISCROLLAREA_H
#define GUISCROLLAREA_H

#include "Bang/Vector2.h"
#include "Bang/GUIMask.h"
#include "Bang/Component.h"

FORWARD class UIGameObject;

class GUIScrollArea : public Component
{
    COMPONENT(GUIScrollArea)

public:
    GUIScrollArea() noexcept;
    virtual ~GUIScrollArea() noexcept;

    void OnUpdate() override;

    void SetMasking(bool masking);
    void SetScrolling(const Vector2i &scrollPx) noexcept;
    void SetScrollingX(int scrollPxX) noexcept;
    void SetScrollingY(int scrollPxY) noexcept;

    bool GetMasking() const;
    UIGameObject* GetContainer() const;
    const Vector2i& GetScrolling() const noexcept;

private:
    GUIMask *p_mask = nullptr;
    UIGameObject *p_childrenContainer = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void UpdateChildrenMargins();

    static UIGameObject* CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

#endif // GUISCROLLAREA_H
