#ifndef ILAYOUTELEMENT_H
#define ILAYOUTELEMENT_H

#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

class ILayoutElement
{
public:
    bool IsInvalid() const;

    void SetPriority(int layoutPriority);

    int GetPriority() const;

    int GetTotalMinWidth()  const;
    int GetTotalMinHeight() const;
    int GetTotalPreferredWidth()  const;
    int GetTotalPreferredHeight() const;
    int GetTotalFlexibleWidth()  const;
    int GetTotalFlexibleHeight() const;

    virtual Vector2i GetTotalMinSize()        const;
    virtual Vector2i GetTotalPreferredSize()  const;
    virtual Vector2i GetTotalFlexiblePxSize() const;
    Vector2i GetTotalSize(LayoutSizeType sizeType) const;

protected:
    ILayoutElement();
    virtual ~ILayoutElement();

    void CalculateTotalSizes() const;
    virtual Vector2i CalculateTotalMinSize()        const = 0;
    virtual Vector2i CalculateTotalPreferredSize()  const = 0;
    virtual Vector2i CalculateTotalFlexiblePxSize() const = 0;

private:
    int m_layoutPriority = 0;
    mutable Vector2i m_calculatedTotalMinSize        = Vector2i::Zero;
    mutable Vector2i m_calculatedTotalPreferredSize  = Vector2i::Zero;
    mutable Vector2i m_calculatedTotalFlexiblePxSize = Vector2i::Zero;

    bool m_isInvalid = true;

    void SetInvalid(bool isInvalid);

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTELEMENT_H
