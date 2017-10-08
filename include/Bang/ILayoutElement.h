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

    void Invalidate();

private:
    int m_layoutPriority = 0;
    mutable Vector2i m_cachedMinSize        = -Vector2i::One;
    mutable Vector2i m_cachedPreferredSize  = -Vector2i::One;

    bool m_isInvalid = true;

    bool IsInvalid() const;
    void SetInvalid(bool isInvalid);

    int GetLayoutPriority() const;

    void CalculateCachedSizes() const;

    Vector2i GetMinSize()       const;
    Vector2i GetPreferredSize() const;
    Vector2  GetFlexibleSize()  const;

    virtual Vector2i _GetMinSize()       const;
    virtual Vector2i _GetPreferredSize() const;
    virtual Vector2  _GetFlexibleSize()  const;

    Vector2 GetSize(LayoutSizeType sizeType) const;

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTELEMENT_H
