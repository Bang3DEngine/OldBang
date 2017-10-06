#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;

class UIScrollBar : public IComponentDriver<UIScrollBar>,
                    public Component
{
    COMPONENT(UIScrollBar)

public:
	UIScrollBar();
	virtual ~UIScrollBar();

    void ScrollTo(float percent);
    void SetLength(int lengthPx);
    void SetThickness(int thickPx);
    void SetVertical(bool vertical);

    UIScrollArea *GetScrollArea();

    bool IsVertical() const;

private:
    GameObject *p_bar = nullptr;
    UIScrollArea *p_scrollArea = nullptr;

    static UIScrollBar* CreateInto(GameObject *go);

    GameObject* GetBar() const;

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollBar>;
};

NAMESPACE_BANG_END

#endif // UISCROLLBAR_H_H

