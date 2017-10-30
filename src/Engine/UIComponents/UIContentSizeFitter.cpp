#include "Bang/UIContentSizeFitter.h"

#include "Bang/Rect.h"
#include "Bang/XMLNode.h"
#include "Bang/UIRenderer.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UIContentSizeFitter::UIContentSizeFitter()
{
}

UIContentSizeFitter::~UIContentSizeFitter()
{

}

void UIContentSizeFitter::ApplyLayout(Axis axis)
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    if (axis == Axis::Horizontal &&
        GetHorizontalSizeType() != LayoutSizeType::None)
    {
        Vector2i hSize (UILayoutManager::GetSize(gameObject, GetHorizontalSizeType()));
        rt->SetWidthFromPivot(hSize.x);
    }

    if (axis == Axis::Vertical &&
        GetVerticalSizeType() != LayoutSizeType::None)
    {
        Vector2i vSize (UILayoutManager::GetSize(gameObject, GetVerticalSizeType()));
        rt->SetHeightFromPivot(vSize.y);
    }
}

void UIContentSizeFitter::SetHorizontalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType == LayoutSizeType::None ||
           sizeType == LayoutSizeType::Min  ||
           sizeType == LayoutSizeType::Preferred);
    m_horizontalSizeType = sizeType;
}

void UIContentSizeFitter::SetVerticalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType == LayoutSizeType::None ||
           sizeType == LayoutSizeType::Min  ||
           sizeType == LayoutSizeType::Preferred);
    m_verticalSizeType = sizeType;
}

LayoutSizeType UIContentSizeFitter::GetHorizontalSizeType() const
{
    return m_horizontalSizeType;
}

LayoutSizeType UIContentSizeFitter::GetVerticalSizeType() const
{
    return m_verticalSizeType;
}

void UIContentSizeFitter::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("HorizontalSizeType"))
    { SetHorizontalSizeType( xmlInfo.Get<LayoutSizeType>("HorizontalSizeType") ); }

    if (xmlInfo.Contains("VerticalSizeType"))
    { SetVerticalSizeType( xmlInfo.Get<LayoutSizeType>("VerticalSizeType") ); }
}

void UIContentSizeFitter::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("HorizontalSizeType", GetHorizontalSizeType());
    xmlInfo->Set("VerticalSizeType", GetVerticalSizeType());
}

void UIContentSizeFitter::OnInvalidated()
{
    IInvalidatable<ILayoutController>::OnInvalidated();

}


