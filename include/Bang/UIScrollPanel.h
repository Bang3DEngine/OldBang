#ifndef UISCROLLPANEL_H
#define UISCROLLPANEL_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

class UIScrollPanel : public IComponentDriver<UIScrollPanel>,
                      public Component
{
    COMPONENT(UIScrollPanel)

public:
	virtual ~UIScrollPanel();

    void OnUpdate() override;

    void SetScrolling(const Vector2i &scrolling);
    void SetScrollingPercent(const Vector2 &scrollingPercent);

    UIScrollArea *GetScrollArea() const;
    UIScrollBar  *GetScrollBar() const;

private:
    constexpr static int WheelScrollSpeedPx = 50;

    UIScrollArea *p_scrollArea = nullptr;
    UIScrollBar  *p_scrollBar  = nullptr;

    UIScrollPanel();

    Vector2 GetContentSize() const;
    Vector2 GetContainerSize() const;

    static UIScrollPanel* CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollPanel>;
};

NAMESPACE_BANG_END

#endif // UISCROLLPANEL_H_H

