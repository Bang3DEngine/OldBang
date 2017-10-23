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

void UIContentSizeFitter::_ApplyLayout()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    Vector2i contentSize = Vector2i::Zero;

    Vector2i hSize (UILayoutManager::GetSize(gameObject, GetHorizontalSizeType()));
    contentSize.x = hSize.x;

    Vector2i vSize (UILayoutManager::GetSize(gameObject, GetVerticalSizeType()));
    contentSize.y = vSize.y;

    if (GetHorizontalSizeType() != LayoutSizeType::None)
    {
        rt->SetWidthFromPivot(contentSize.x);
    }

    if (GetVerticalSizeType() != LayoutSizeType::None)
    {
        rt->SetHeightFromPivot(contentSize.y);
    }
}

void UIContentSizeFitter::SetHorizontalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType == LayoutSizeType::Min ||
           sizeType == LayoutSizeType::Preferred);
    m_horizontalSizeType = sizeType;
}

void UIContentSizeFitter::SetVerticalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType == LayoutSizeType::Min ||
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


