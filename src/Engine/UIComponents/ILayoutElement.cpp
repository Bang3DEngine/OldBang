#include "Bang/ILayoutElement.h"

#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

ILayoutElement::ILayoutElement() {}
ILayoutElement::~ILayoutElement() {}

void ILayoutElement::SetIgnoreLayout(bool ignoreLayout)
{
    m_ignoreLayout = ignoreLayout;
}

void ILayoutElement::SetLayoutPriority(int layoutPriority)
{
    m_layoutPriority = layoutPriority;
}

void ILayoutElement::OnInvalidated()
{
    IInvalidatable<ILayoutElement>::OnInvalidated();
    UILayoutManager::OnInvalidated(this);
}

bool ILayoutElement::GetIgnoreLayout() const
{
    return m_ignoreLayout;
}

int ILayoutElement::GetLayoutPriority() const
{
    return m_layoutPriority;
}

Vector2i ILayoutElement::GetMinSize() const
{
    CalculateCachedSizes();
    return m_cachedMinSize;
}

Vector2i ILayoutElement::GetPreferredSize() const
{
    CalculateCachedSizes();
    return m_cachedPreferredSize;
}

Vector2 ILayoutElement::GetFlexibleSize() const
{
    return _GetFlexibleSize();
}

Vector2i ILayoutElement::_GetMinSize() const
{
    return Vector2i(-1);
}

Vector2i ILayoutElement::_GetPreferredSize() const
{
    return Vector2i(-1);
}

Vector2 ILayoutElement::_GetFlexibleSize() const
{
    return Vector2(-1);
}

Vector2 ILayoutElement::GetSize(LayoutSizeType sizeType) const
{
    if (sizeType == LayoutSizeType::Min)
    { return Vector2( GetMinSize() ); }

    if (sizeType == LayoutSizeType::Preferred)
    { return Vector2( GetPreferredSize() ); }

    if (sizeType == LayoutSizeType::Flexible)  { return GetFlexibleSize(); }

    return Vector2::Zero;
}

void ILayoutElement::CalculateCachedSizes() const
{
    // if (IsInvalid())
    {
        m_cachedMinSize       = _GetMinSize();
        m_cachedPreferredSize = _GetPreferredSize();
        Validate();
    }
}
