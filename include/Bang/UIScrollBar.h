#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

#include "Bang/Alignment.h"
#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIButton;
FORWARD class UIScrollArea;

class UIScrollBar : public IComponentDriver<UIScrollBar>,
                    public Component
{
    COMPONENT(UIScrollBar)

public:
	UIScrollBar();
	virtual ~UIScrollBar();

    void OnUpdate() override;

    void SetSide(Side side);
    void SetScrolling(int scrollingPx);
    void SetScrollingPercent(float percent);
    void SetLength(int lengthPx);
    void SetLengthPercent(float lengthPercent);
    void SetThickness(int thickPx);

    Side GetSide() const;
    int GetScrolling() const;
    float GetScrollingPercent() const;
    int GetLength() const;
    int GetThickness() const;
    Axis GetScrollAxis() const;

private:
    int m_length = 0;
    int m_thickness = 0;
    int m_scrollingPx = 0;
    bool m_wasGrabbed = false;
    Side m_side = Undef<Side>();
    Vector2i m_grabOffset = Vector2i::Zero;

    GameObject *p_bar = nullptr;
    UIButton *p_button = nullptr;
    UIScrollArea *p_scrollArea = nullptr;

    static UIScrollBar* CreateInto(GameObject *go);

    void UpdateLengthThicknessMargins();

    int GetScrollingSpacePx() const;
    Rect GetScrollingRect() const;
    UIScrollArea *GetScrollArea() const;
    UIButton* GetButton() const;
    GameObject* GetBar() const;

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollBar>;
};

NAMESPACE_BANG_END

#endif // UISCROLLBAR_H_H

