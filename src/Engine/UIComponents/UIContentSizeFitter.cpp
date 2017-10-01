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

void UIContentSizeFitter::ApplyLayout()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    Vector2i contentSize = Vector2i::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i hSize (UILayoutManager::GetSize(child, GetHorizontalSizeType()));
        if (GetHorizontalSizeFitMode() == SizeFitMode::Sum)
        {
            contentSize.x += hSize.x;
        }
        else if (GetHorizontalSizeFitMode() == SizeFitMode::Max)
        {
            contentSize.x = Math::Max(contentSize.x, hSize.x);
        }

        Vector2i vSize (UILayoutManager::GetSize(child, GetVerticalSizeType()));
        if (GetVerticalSizeFitMode() == SizeFitMode::Sum)
        {
            contentSize.y += vSize.y;
        }
        else if (GetVerticalSizeFitMode() == SizeFitMode::Max)
        {
            contentSize.y = Math::Max(contentSize.y, vSize.y);
        }
    }

    if (GetHorizontalSizeType() != LayoutSizeType::None)
    {
        rt->SetWidthFromPivot(contentSize.x);
    }

    if (GetVerticalSizeType() != LayoutSizeType::None)
    {
        rt->SetHeightFromPivot(contentSize.y);
    }
}

void UIContentSizeFitter::SetHorizontalSizeFitMode(SizeFitMode fitMode)
{
    m_horizontalFitMode = fitMode;
}

void UIContentSizeFitter::SetVerticalSizeFitMode(SizeFitMode fitMode)
{
    m_verticalFitMode = fitMode;
}

void UIContentSizeFitter::SetHorizontalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType != LayoutSizeType::Flexible);
    m_horizontalSizeType = sizeType;
}

void UIContentSizeFitter::SetVerticalSizeType(LayoutSizeType sizeType)
{
    ASSERT(sizeType != LayoutSizeType::Flexible);
    m_verticalSizeType = sizeType;
}

SizeFitMode UIContentSizeFitter::GetHorizontalSizeFitMode() const
{
    return m_horizontalFitMode;
}

SizeFitMode UIContentSizeFitter::GetVerticalSizeFitMode() const
{
    return m_verticalFitMode;
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


