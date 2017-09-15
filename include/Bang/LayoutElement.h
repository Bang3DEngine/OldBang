#ifndef LAYOUTELEMENT_H
#define LAYOUTELEMENT_H

#include "Bang/Map.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class LayoutElement : public Component
{
    COMPONENT(LayoutElement)

public:
    void SetMinWidth(int minWidth);
    void SetMinHeight(int minHeight);
    void SetMinSize(const Vector2i& minSize);
    void SetPreferredWidth(int minWidth);
    void SetPreferredHeight(int minHeight);
    void SetPreferredSize(const Vector2i& minSize);
    void SetFlexibleWidth(int minWidth);
    void SetFlexibleHeight(int minHeight);
    void SetFlexibleSize(const Vector2i& minSize);

    int GetMinWidth() const;
    int GetMinHeight() const;
    Vector2i GetMinSize() const;
    int GetPreferredWidth() const;
    int GetPreferredHeight() const;
    Vector2i GetPreferredSize() const;
    float GetFlexibleWidth() const;
    float GetFlexibleHeight() const;
    Vector2 GetFlexibleSize() const;

protected:
    LayoutElement();
    virtual ~LayoutElement();

private:
    int m_minWidth         = 10;
    int m_minHeight        = 10;
    int m_preferredWidth   = 20;
    int m_preferredHeight  = 20;
    float m_flexibleWidth  = 0.0f;
    float m_flexibleHeight = 0.0f;
};

NAMESPACE_BANG_END

#endif // LAYOUTELEMENT_H
