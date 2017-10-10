#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

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

    void SetScrollingPercent(float percent);
    void SetLength(int lengthPx);
    void SetLengthPercent(float lengthPercent);
    void SetThickness(int thickPx);
    void SetVertical(bool vertical);

    float GetScrollingPercent() const;
    int GetLength() const;
    int GetThickness() const;
    bool IsVertical() const;

private:
    bool m_wasGrabbed = false;
    float m_scrollingPercent = 0.0f;
    Vector2i m_grabOffset = Vector2i::Zero;

    GameObject *p_bar = nullptr;
    UIButton *p_button = nullptr;
    UIScrollArea *p_scrollArea = nullptr;

    static UIScrollBar* CreateInto(GameObject *go);

    Recti GetScrollingRect() const;
    UIScrollArea *GetScrollArea() const;
    UIButton* GetButton() const;
    GameObject* GetBar() const;

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollBar>;
};

NAMESPACE_BANG_END

#endif // UISCROLLBAR_H_H

