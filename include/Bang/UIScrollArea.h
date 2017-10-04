#ifndef UISCROLLAREA_H
#define UISCROLLAREA_H

#include "Bang/Vector2.h"
#include "Bang/UIMask.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;

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
    GameObject* GetContainer() const;
    const Vector2i& GetScrolling() const noexcept;

private:
    UIMask *p_mask = nullptr;
    GameObject *p_childrenContainer = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void UpdateChildrenMargins();

    static GameObject* CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UISCROLLAREA_H
