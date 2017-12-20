#include "Bang/UILayoutElement.h"

#include "Bang/GameObject.h"
#include "Bang/UILayoutManager.h"
#include "Bang/ILayoutController.h"

USING_NAMESPACE_BANG

UILayoutElement::UILayoutElement()
{
    SetLayoutPriority(1);
}

UILayoutElement::~UILayoutElement()
{

}

void UILayoutElement::SetMinWidth(int minWidth)
{
    SetMinSize( Vector2i(minWidth, GetMinHeight()) );
}

void UILayoutElement::SetMinHeight(int minHeight)
{
    SetMinSize( Vector2i(GetMinWidth(), minHeight) );
}

void UILayoutElement::SetMinSize(const Vector2i &minSize)
{
    if (minSize != m_minSize)
    {
        m_minSize = minSize;
        OnChanged();
        SetPreferredSize(Vector2i::Max(GetMinSize(), GetPreferredSize()));
    }
}

void UILayoutElement::SetPreferredWidth(int preferredWidth)
{
    SetPreferredSize( Vector2i(preferredWidth, GetPreferredHeight()) );
}

void UILayoutElement::SetPreferredHeight(int preferredHeight)
{
    SetPreferredSize( Vector2i(GetPreferredWidth(), preferredHeight) );
}

void UILayoutElement::SetPreferredSize(const Vector2i &preferredSize)
{
    if (preferredSize != m_preferredSize)
    {
        m_preferredSize = preferredSize;
        OnChanged();
    }
}

void UILayoutElement::SetFlexibleWidth(float flexibleWidth)
{
    SetFlexibleSize( Vector2(flexibleWidth, GetFlexibleHeight()) );
}

void UILayoutElement::SetFlexibleHeight(float flexibleHeight)
{
    SetFlexibleSize( Vector2(GetFlexibleWidth(), flexibleHeight) );
}

void UILayoutElement::SetFlexibleSize(const Vector2 &flexibleSize)
{
    if (flexibleSize != m_flexibleSize)
    {
        m_flexibleSize = flexibleSize;
        OnChanged();
    }
}


int UILayoutElement::GetMinWidth() const
{
    return GetMinSize().x;
}

int UILayoutElement::GetMinHeight() const
{
    return GetMinSize().y;
}

const Vector2i& UILayoutElement::GetMinSize() const
{
    return m_minSize;
}

int UILayoutElement::GetPreferredWidth() const
{
    return GetPreferredSize().x;
}

int UILayoutElement::GetPreferredHeight() const
{
    return GetPreferredSize().y;
}

Vector2i UILayoutElement::GetPreferredSize() const
{
    return m_preferredSize;
}

float UILayoutElement::GetFlexibleWidth() const
{
    return GetFlexibleSize().x;
}

float UILayoutElement::GetFlexibleHeight() const
{
    return GetFlexibleSize().y;
}

Vector2 UILayoutElement::GetFlexibleSize() const
{
    return m_flexibleSize;
}

void UILayoutElement::CalculateLayout(Axis axis)
{
    SetCalculatedLayout(axis, GetMinSize().GetAxis(axis),
                        GetPreferredSize().GetAxis(axis));
}

void UILayoutElement::OnChanged()
{
    ILayoutElement::Invalidate();
    /*
    auto parentLayoutControllers =
                GetGameObject()->GetComponentsInParent<ILayoutController>();
    for (ILayoutController *controller : parentLayoutControllers)
    {
        controller->Invalidate();
    }
    */
}
