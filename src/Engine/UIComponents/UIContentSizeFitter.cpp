#include "Bang/UIContentSizeFitter.h"

#include "Bang/Rect.h"
#include "Bang/XMLNode.h"
#include "Bang/UIRenderer.h"
#include "Bang/GameObject.h"
#include "Bang/UIDirLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG

UIContentSizeFitter::UIContentSizeFitter()
{
}

UIContentSizeFitter::~UIContentSizeFitter()
{

}

void UIContentSizeFitter::SetHorizontalSizeFitMode(SizeFitMode fitMode)
{
    m_horizontalFitMode = fitMode;
}

void UIContentSizeFitter::SetVerticalSizeFitMode(SizeFitMode fitMode)
{
    m_verticalFitMode = fitMode;
}

void UIContentSizeFitter::ApplyLayout()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    List<ILayoutElement*> layoutElms =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    Vector2i contentSize = Vector2i::Zero;
    for (ILayoutElement *layoutElm : layoutElms)
    {
        Vector2i hTotalSize = layoutElm->GetTotalSize(GetHorizontalSizeType());
        if (GetHorizontalSizeFitMode() == SizeFitMode::Sum)
        {
            contentSize.x += hTotalSize.x;
        }
        else if (GetHorizontalSizeFitMode() == SizeFitMode::Max)
        {
            contentSize.x = Math::Max(contentSize.x, hTotalSize.x);
        }

        Vector2i vTotalSize = layoutElm->GetTotalSize(GetVerticalSizeType());
        if (GetVerticalSizeFitMode() == SizeFitMode::Sum)
        {
            contentSize.y += vTotalSize.y;
        }
        else if (GetVerticalSizeFitMode() == SizeFitMode::Max)
        {
            contentSize.y = Math::Max(contentSize.y, vTotalSize.y);
        }
    }

    UIDirLayout *dirLayout = gameObject->GetComponentInChildren<UIDirLayout>(false);
    if (dirLayout) { contentSize += dirLayout->GetTotalSpacing(); }

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
    m_horizontalSizeType = sizeType;
}

void UIContentSizeFitter::SetVerticalSizeType(LayoutSizeType sizeType)
{
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


