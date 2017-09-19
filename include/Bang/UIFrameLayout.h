#ifndef UIFRAMELAYOUT_H
#define UIFRAMELAYOUT_H

#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

class UIFrameLayout : public Component,
                      public ILayoutElement,
                      public ILayoutController
{
    COMPONENT(UIFrameLayout);

public:
    UIFrameLayout();
    virtual ~UIFrameLayout();

    void SetPaddingLeft(int paddingLeft);
    void SetPaddingBot(int paddingBot);
    void SetPaddingRight(int paddingRight);
    void SetPaddingTop(int paddingTop);
    void SetPaddings(int padding);
    void SetPaddings(int paddingLeft, int paddingBot,
                     int paddingRight, int paddingTop);

    int GetPaddingLeft()  const;
    int GetPaddingBot()   const;
    int GetPaddingRight() const;
    int GetPaddingTop()   const;
    const Vector2i &GetPaddingLeftBot()  const;
    const Vector2i &GetPaddingRightTop() const;
    Vector2i GetPaddingSize() const;

    // ILayoutController
    virtual void ApplyLayout() override;

    // ILayoutElement
    virtual Vector2i CalculateTotalMinSize() const override;
    virtual Vector2i CalculateTotalPreferredSize() const override;
    virtual Vector2i CalculateTotalFlexiblePxSize() const override;

private:
    Vector2i m_paddingLeftBot  = Vector2i::Zero;
    Vector2i m_paddingRightTop = Vector2i::Zero;
};

NAMESPACE_BANG_END

#endif // UIFRAMELAYOUT_H
