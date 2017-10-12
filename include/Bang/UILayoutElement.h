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
    Vector2i GetPreferredSize() const;
    float GetFlexibleWidth() const;
    float GetFlexibleHeight() const;
    const Vector2& GetFlexibleSize() const;

protected:
    Vector2i _GetMinSize()       const override;
    Vector2i _GetPreferredSize() const override;
    Vector2  _GetFlexibleSize()  const override;

private:
    Vector2i m_minSize       = -Vector2i::One;
    Vector2i m_preferredSize = -Vector2i::One;
    Vector2  m_flexibleSize  = -Vector2::One;

    void OnChanged();
};

NAMESPACE_BANG_END

#endif // UILAYOUTELEMENT_H
