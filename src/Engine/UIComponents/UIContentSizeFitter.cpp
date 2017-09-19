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

void UIContentSizeFitter::ApplyLayout()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    List<ILayoutElement*> layoutElms =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    Vector2i contentSize = Vector2i::Zero;
    for (ILayoutElement *layoutElm : layoutElms)
    {
        contentSize.x += layoutElm->GetTotalSize(GetHorizontalSizeType()).x;
        contentSize.y += layoutElm->GetTotalSize(GetVerticalSizeType()).y;
    }

    UIDirLayout *dirLayout = gameObject->GetComponentInChildren<UIDirLayout>(false);
    if (dirLayout) { contentSize += dirLayout->GetTotalSpacing(); }

    if (GetHorizontalSizeType() != LayoutSizeType::None)
    {
        rt->SetMarginLeft (0);
        rt->SetMarginRight(-contentSize.x);
    }

    if (GetVerticalSizeType() != LayoutSizeType::None)
    {
        rt->SetMarginTop(0);
        rt->SetMarginBot(-contentSize.y);
    }
}

void UIContentSizeFitter::SetHorizontalSizeType(LayoutSizeType sizeType)
{
    m_horizontalSizeType = sizeType;
}

LayoutSizeType UIContentSizeFitter::GetHorizontalSizeType() const
{
    return m_horizontalSizeType;
}

void UIContentSizeFitter::SetVerticalSizeType(LayoutSizeType sizeType)
{
    m_verticalSizeType = sizeType;
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


