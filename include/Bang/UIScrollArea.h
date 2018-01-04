#ifndef UISCROLLAREA_H
#define UISCROLLAREA_H

#include "Bang/Vector2.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class UIRectMask;
FORWARD class UIImageRenderer;

class UIScrollArea : public Component
{
    COMPONENT(UIScrollArea)

public:
    virtual ~UIScrollArea();

    void OnUpdate() override;

    void SetScrolling(const Vector2i &scrollPx);
    void SetScrollingX(int scrollPxX);
    void SetScrollingY(int scrollPxY);

    void SetContainedGameObject(GameObject *go);

    UIRectMask* GetMask() const;
    GameObject* GetContainer() const;
    GameObject* GetContainedGameObject() const;
    UIImageRenderer* GetBackground() const;
    const Vector2i& GetScrolling() const;

private:
    UIScrollArea();

    UIRectMask *p_mask = nullptr;
    UIImageRenderer *p_bg = nullptr;
    GameObject *p_container = nullptr;
    GameObject *p_containedGo = nullptr;

    Vector2i m_scrollingPx = Vector2i::Zero;

    void UpdatePaddings();

    static UIScrollArea* CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UISCROLLAREA_H
