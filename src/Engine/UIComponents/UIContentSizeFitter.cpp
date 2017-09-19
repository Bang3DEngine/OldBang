#include "Bang/UIContentSizeFitter.h"

#include "Bang/Rect.h"
#include "Bang/XMLNode.h"
#include "Bang/UIRenderer.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
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

    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    Recti boundingRectPx;
    bool firstRect = true;
    List<UIRenderer*> uiRenderers = gameObject->GetComponents<UIRenderer>();
    for (UIRenderer *uiRend : uiRenderers)
    {
        Recti partBoundingRect =
             GL::FromGlobalNDCToPixels( uiRend->GetBoundingRect(nullptr) );
        if (!firstRect)
        {
            boundingRectPx = Recti::Union(boundingRectPx, partBoundingRect);
        }
        else { boundingRectPx = partBoundingRect; firstRect = false; }
    }

    Recti rtRectPx = rt->GetScreenSpaceRectPx();
    rt->SetAnchors( Vector2(-1) );
    rt->SetMarginLeft (  (rtRectPx.GetMin().x - boundingRectPx.GetMin().x) );
    rt->SetMarginRight( -(rtRectPx.GetMin().x + boundingRectPx.GetWidth()) );
    rt->SetMarginBot  (  (rtRectPx.GetMin().y - boundingRectPx.GetMin().y) );
    rt->SetMarginTop  ( -(rtRectPx.GetMin().y + boundingRectPx.GetHeight()));

    /*
    rt->SetMarginLeft (  (boundingRectPx.GetMin().x) );
    rt->SetMarginRight(  (-boundingRectPx.GetMax().x) );
    rt->SetMarginBot  (  (boundingRectPx.GetMin().y) );
    rt->SetMarginTop  (  (-boundingRectPx.GetMax().y) );
    */

    Debug_Peek(rt->GetScreenSpaceRectPx());
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


