#ifndef UILAYOUTELEMENT_H
#define UILAYOUTELEMENT_H

#include "Bang/Map.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UILayoutElement : public Component
{
    COMPONENT(UILayoutElement)

public:
    void SetMinWidth(int minWidth);
    void SetMinHeight(int minHeight);
    void SetMinSize(const Vector2i& minSize);
    void SetPreferredWidth(int preferredWidth);
    void SetPreferredHeight(int preferredHeight);
    void SetPreferredSize(const Vector2i& preferredSize);
    void SetFlexibleWidth(int flexibleWidth);
    void SetFlexibleHeight(int flexibleHeight);
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

protected:
    UILayoutElement();
    virtual ~UILayoutElement();

private:
    Vector2i m_minSize       = Vector2i(50);
    Vector2i m_preferredSize = Vector2i(100);
    Vector2  m_flexibleSize  = Vector2::Zero;
};

NAMESPACE_BANG_END

#endif // UILAYOUTELEMENT_H
