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
    virtual ~UIScrollArea();

    void OnUpdate() override;

    void SetScrolling(const Vector2i &scrollPx);
    void SetScrollingX(int scrollPxX);
    void SetScrollingY(int scrollPxY);

    UIMask* GetMask() const;
    GameObject* GetContainer() const;
    const Vector2i& GetScrolling() const;

private:
    UIScrollArea();

    UIMask *p_mask = nullptr;
    GameObject *p_childrenContainer = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void UpdatePaddings();

    static UIScrollArea* CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UISCROLLAREA_H
