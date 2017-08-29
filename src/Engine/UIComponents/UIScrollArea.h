#ifndef UISCROLLAREA_H
#define UISCROLLAREA_H

#include "Bang/Vector2.h"
#include "Bang/UIMask.h"
#include "Bang/Component.h"

FORWARD class UIGameObject;

class UIScrollArea : public Component
{
    COMPONENT(UIScrollArea)

public:
    UIScrollArea() noexcept;
    virtual ~UIScrollArea() noexcept;

    void OnUpdate() override;

    void SetMasking(bool masking);
    void SetScrolling(const Vector2i &scrollPx) noexcept;
    void SetScrollingX(int scrollPxX) noexcept;
    void SetScrollingY(int scrollPxY) noexcept;

    bool IsMasking() const;
    UIGameObject* GetContainer() const;
    const Vector2i& GetScrolling() const noexcept;

private:
    UIMask *p_mask = nullptr;
    UIGameObject *p_childrenContainer = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void UpdateChildrenMargins();

    static UIGameObject* CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

#endif // UISCROLLAREA_H
