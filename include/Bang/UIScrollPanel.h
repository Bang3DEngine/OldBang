#ifndef UISCROLLPANEL_H
#define UISCROLLPANEL_H

#include "Bang/Alignment.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollBar;
FORWARD class UIScrollArea;

enum class ShowScrollMode { Never = 0, WhenNeeded = 1, Always = 2 };

class UIScrollPanel : public Component
{
    COMPONENT(UIScrollPanel)

public:
	virtual ~UIScrollPanel();

    void OnUpdate() override;

    void SetVerticalScrollBarSide(HorizontalSide side);
    void SetHorizontalScrollBarSide(VerticalSide side);
    void SetVerticalShowScrollMode(ShowScrollMode showScrollMode);
    void SetHorizontalShowScrollMode(ShowScrollMode showScrollMode);
    void SetVerticalScrollEnabled(bool enabled);
    void SetHorizontalScrollEnabled(bool enabled);
    void SetScrolling(const Vector2i &scrolling);
    void SetScrollingPercent(const Vector2 &scrollingPercent);

    Vector2i GetScrolling() const;
    HorizontalSide GetVerticalScrollBarSide() const;
    VerticalSide GetHorizontalScrollBarSide() const;
    ShowScrollMode GetVerticalShowScrollMode() const;
    ShowScrollMode GetHorizontalShowScrollMode() const;
    bool IsVerticalScrollEnabled() const;
    bool IsHorizontalScrollEnabled() const;
    UIScrollArea *GetScrollArea() const;
    UIScrollBar  *GetVerticalScrollBar() const;
    UIScrollBar  *GetHorizontalScrollBar() const;
    Vector2 GetContentSize() const;
    Vector2 GetContainerSize() const;
    Vector2 GetMaxScrollLength() const;

private:
    const static float WheelScrollSpeedPx;

    UIScrollArea *p_scrollArea = nullptr;
    UIScrollBar  *p_verticalScrollBar = nullptr;
    UIScrollBar  *p_horizontalScrollBar = nullptr;
    HorizontalSide m_verticalScrollBarSide = Undef<HorizontalSide>();
    VerticalSide m_horizontalScrollBarSide = Undef<VerticalSide>();
    ShowScrollMode m_verticalShowScrollMode = Undef<ShowScrollMode>();
    ShowScrollMode m_horizontalShowScrollMode = Undef<ShowScrollMode>();
    bool m_verticalScrollEnabled = true;
    bool m_horizontalScrollEnabled = true;

    UIScrollPanel();

    void UpdateScrollUI();

    void HandleScrollAreaRectTransform();
    void HandleScrollShowMode(const Vector2& contentSize,
                              const Vector2& containerSize);

    static UIScrollPanel* CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UISCROLLPANEL_H_H

