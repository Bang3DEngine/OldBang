#ifndef UISCROLLPANEL_H
#define UISCROLLPANEL_H

#include "Bang/Alignment.h"
#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

enum class ShowScrollMode { Never = 0, WhenNeeded = 1, Always = 2 };

class UIScrollPanel : public IComponentDriver<UIScrollPanel>,
                      public Component
{
    COMPONENT(UIScrollPanel)

public:
	virtual ~UIScrollPanel();

    void OnUpdate() override;

    void SetScrollBarSide(VerticalSide side);
    void SetScrollBarSide(HorizontalSide side);
    void SetVerticalShowScrollMode(ShowScrollMode showScrollMode);
    void SetHorizontalShowScrollMode(ShowScrollMode showScrollMode);
    void SetScrolling(const Vector2i &scrolling);
    void SetScrollingPercent(const Vector2 &scrollingPercent);

    VerticalSide GetVerticalScrollBarSide() const;
    HorizontalSide GetHorizontalScrollBarSide() const;
    ShowScrollMode GetVerticalShowScrollMode() const;
    ShowScrollMode GetHorizontalShowScrollMode() const;
    UIScrollArea *GetScrollArea() const;
    UIScrollBar  *GetScrollBar() const;

private:
    constexpr static int WheelScrollSpeedPx = 50;

    UIScrollArea *p_scrollArea = nullptr;
    UIScrollBar  *p_scrollBar  = nullptr;
    VerticalSide m_verticalScrollBarSide = Undef<VerticalSide>();
    HorizontalSide m_horizontalScrollBarSide = Undef<HorizontalSide>();
    ShowScrollMode m_verticalShowScrollMode = Undef<ShowScrollMode>();
    ShowScrollMode m_horizontalShowScrollMode = Undef<ShowScrollMode>();

    UIScrollPanel();

    void HandleScrollAreaRectTransform();
    void HandleScrollShowMode(int contentSize, int containerSize);

    Vector2 GetContentSize() const;
    Vector2 GetContainerSize() const;

    static UIScrollPanel* CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollPanel>;
};

NAMESPACE_BANG_END

#endif // UISCROLLPANEL_H_H

