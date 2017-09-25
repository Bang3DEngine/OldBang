#ifndef UILAYOUTELEMENT_H
#define UILAYOUTELEMENT_H

#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"

NAMESPACE_BANG_BEGIN

class UILayoutElement : public Component,
                        public ILayoutElement
{
    COMPONENT(UILayoutElement);

public:
    UILayoutElement();
    virtual ~UILayoutElement();

    void SetMinWidth(int minWidth);
    void SetMinHeight(int minHeight);
    void SetMinSize(const Vector2i& minSize);
    void SetPreferredWidth(int preferredWidth);
    void SetPreferredHeight(int preferredHeight);
    void SetPreferredSize(const Vector2i& preferredSize);
    void SetFlexibleWidth(float flexibleWidth);
    void SetFlexibleHeight(float flexibleHeight);
    void SetFlexibleSize(const Vector2& flexibleSize);

    int GetMinWidth() const;
    int GetMinHeight() const;
    const Vector2i& GetMinSize() const;
    int GetPreferredWidth() const;
    int GetPreferredHeight() const;
    const Vector2i& GetPreferredSize() const;
    float GetFlexibleWidth() const;
    float GetFlexibleHeight() const;
    const Vector2& GetFlexibleSize() const;
    int GetFlexiblePxWidth() const;
    int GetFlexiblePxHeight() const;
    Vector2i GetFlexiblePxSize() const;

protected:
    Vector2i CalculateTotalMinSize() const override;
    Vector2i CalculateTotalPreferredSize() const override;
    Vector2i CalculateTotalFlexiblePxSize() const override;

private:
    Vector2i m_minSize       = Vector2i::Zero;
    Vector2i m_preferredSize = Vector2i::Zero;
    Vector2  m_flexibleSize  = Vector2::Zero;
};

NAMESPACE_BANG_END

#endif // UILAYOUTELEMENT_H
