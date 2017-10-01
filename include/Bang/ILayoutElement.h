#ifndef ILAYOUTELEMENT_H
#define ILAYOUTELEMENT_H

#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

class ILayoutElement
{
public:
    void SetLayoutPriority(int layoutPriority);

protected:
    ILayoutElement();
    virtual ~ILayoutElement();

    void CalculateTotalSizes() const;
    virtual Vector2i CalculateTotalMinSize()        const = 0;
    virtual Vector2i CalculateTotalPreferredSize()  const = 0;

private:
    int m_layoutPriority = 0;
    mutable Vector2i m_calculatedTotalMinSize        = Vector2i::Zero;
    mutable Vector2i m_calculatedTotalPreferredSize  = Vector2i::Zero;

    bool m_isInvalid = true;

    bool IsInvalid() const;
    void SetInvalid(bool isInvalid);

    int GetLayoutPriority() const;

    virtual Vector2i GetTotalMinSize()       const;
    virtual Vector2i GetTotalPreferredSize() const;
    virtual Vector2  GetFlexibleSize()       const;

    Vector2 GetTotalSize(LayoutSizeType sizeType) const;

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTELEMENT_H
