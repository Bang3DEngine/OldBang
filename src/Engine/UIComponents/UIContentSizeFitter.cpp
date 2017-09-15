#include "Bang/UIContentSizeFitter.h"

#include "Bang/Rect.h"
#include "Bang/XMLNode.h"
#include "Bang/UIRenderer.h"
#include "Bang/GameObject.h"
#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG

UIContentSizeFitter::UIContentSizeFitter()
{
}

UIContentSizeFitter::~UIContentSizeFitter()
{

}

void UIContentSizeFitter::OnUpdate()
{
    Component::OnUpdate();

    UILayoutElement *layoutElm = gameObject->GetComponent<UILayoutElement>();
    ENSURE(layoutElm);

    Recti boundingRect;
    bool firstRect = true;
    List<UIRenderer*> uiRenderers = gameObject->GetComponents<UIRenderer>();
    for (UIRenderer *uiRend : uiRenderers)
    {
        Recti childBoundingRect =
             GL::FromGlobalNDCToPixels( uiRend->GetBoundingRect(nullptr) );
        if (!firstRect)
        {
            boundingRect = Recti::Union(boundingRect, childBoundingRect);
        }
        else { boundingRect = childBoundingRect; firstRect = false; }
    }

    if (GetHorizontalSizeFit() == SizeFit::Min ||
        GetHorizontalSizeFit() == SizeFit::Both)
    { layoutElm->SetMinWidth( boundingRect.GetWidth() ); }

    if (GetVerticalSizeFit() == SizeFit::Min ||
        GetVerticalSizeFit() == SizeFit::Both)
    { layoutElm->SetMinHeight( boundingRect.GetHeight() ); }

    if (GetHorizontalSizeFit() == SizeFit::Preferred ||
        GetHorizontalSizeFit() == SizeFit::Both)
    { layoutElm->SetPreferredWidth( boundingRect.GetWidth() ); }

    if (GetHorizontalSizeFit() == SizeFit::Preferred ||
        GetHorizontalSizeFit() == SizeFit::Both)
    { layoutElm->SetPreferredHeight( boundingRect.GetHeight() ); }
}

void UIContentSizeFitter::SetHorizontalSizeFit(SizeFit sizeFit)
{
    m_horizontalSizeFit = sizeFit;
}

SizeFit UIContentSizeFitter::GetHorizontalSizeFit() const
{
    return m_horizontalSizeFit;
}

void UIContentSizeFitter::SetVerticalSizeFit(SizeFit sizeFit)
{
    m_verticalSizeFit = sizeFit;
}

SizeFit UIContentSizeFitter::GetVerticalSizeFit() const
{
    return m_verticalSizeFit;
}

void UIContentSizeFitter::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("HorizontalSizeFit"))
    { SetHorizontalSizeFit( xmlInfo.Get<SizeFit>("HorizontalSizeFit") ); }

    if (xmlInfo.Contains("VerticalSizeFit"))
    { SetVerticalSizeFit( xmlInfo.Get<SizeFit>("VerticalSizeFit") ); }
}

void UIContentSizeFitter::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("HorizontalSizeFit", GetHorizontalSizeFit());
    xmlInfo->Set("VerticalSizeFit", GetVerticalSizeFit());
}


