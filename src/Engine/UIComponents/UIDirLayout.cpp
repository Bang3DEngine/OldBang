#include "Bang/UIDirLayout.h"

#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/XMLNode.h"
#include "Bang/UIGameObject.h"
#include "Bang/LayoutElement.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIDirLayout::UIDirLayout() : UIDirLayout(false)
{
}

UIDirLayout::UIDirLayout(bool vertical) :
    m_vertical(vertical)
{
}

UIDirLayout::~UIDirLayout()
{
}

void UIDirLayout::OnUpdate()
{
    Component::OnUpdate();

    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    Recti layoutRect = rt->GetScreenSpaceRectPx();
    const int numChildren = gameObject->GetChildren().Size();

    Array<int> childrenSizes(numChildren, 0);

    // Apply minSizes
    int i = 0;
    int occupiedPixels = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        LayoutElement *cle = child->GetComponent<LayoutElement>();
        if (!cle) { continue; }

        if (m_vertical)
        {
            childrenSizes[i] = cle->GetMinHeight();
            occupiedPixels += cle->GetMinHeight();
        }
        else
        {
            childrenSizes[i] = cle->GetMinWidth();
            occupiedPixels += cle->GetMinWidth();
        }
        ++i;
    }

    // Apply preferredSizes
    i = 0;
    int pxSizeAvailableSize = ( (m_vertical ? layoutRect.GetHeight() :
                                              layoutRect.GetWidth() )
                                 - occupiedPixels);
    for (GameObject *child : gameObject->GetChildren())
    {
        LayoutElement *cle = child->GetComponent<LayoutElement>();
        if (!cle) { continue; }

        int pxToAdd = 0;
        if (m_vertical)
        {
            pxToAdd = Math::Min(pxSizeAvailableSize,
                                cle->GetPreferredHeight() - cle->GetMinHeight());
        }
        else
        {
            pxToAdd = Math::Min(pxSizeAvailableSize,
                                cle->GetPreferredWidth() - cle->GetMinWidth());
        }

        pxToAdd = Math::Max(pxToAdd, 0);
        childrenSizes[i]    += pxToAdd;
        pxSizeAvailableSize -= pxToAdd;
        ++i;
    }

    i = 0;
    int marginUntilNow = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        int childSize = childrenSizes[i];
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (m_vertical)
        {
            crt->SetAnchorMin( Vector2(crt->GetAnchorMin().x, -1.0f) );
            crt->SetAnchorMax( Vector2(crt->GetAnchorMax().x, -1.0f) );
            crt->SetMarginTop(  marginUntilNow );
            crt->SetMarginBot( -marginUntilNow - childSize );
        }
        else
        {
            crt->SetAnchorMin( Vector2(-1.0f, crt->GetAnchorMin().y) );
            crt->SetAnchorMax( Vector2(-1.0f, crt->GetAnchorMax().y) );
            crt->SetMarginLeft (  marginUntilNow );
            crt->SetMarginRight( -marginUntilNow - childSize );
        }
        marginUntilNow += childSize;
        ++i;
    }
}

void UIDirLayout::SetSpacing(int spacingPx)
{
    m_spacingPx = spacingPx;
}

void UIDirLayout::SetStretch(int index, float stretch)
{
    m_stretches[index] = stretch;
}

float UIDirLayout::GetStretch(int index) const
{
    if (!m_stretches.ContainsKey(index)) { return 1.0f; }
    return m_stretches.At(index);
}

void UIDirLayout::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("SpacingPx"))
    { SetSpacing( xmlInfo.Get<int>("SpacingPx") ); }

    int i = 0;
    while (xmlInfo.Contains("Stretch" + String(i)))
    {
        SetStretch(i, xmlInfo.Get<float>("Stretch" + String(i)));
        ++i;
    }
}

void UIDirLayout::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("SpacingPx", m_spacingPx);

    const Map<int, float> &stretches = m_stretches;
    for (auto it = stretches.cbegin(); it != stretches.cend(); ++it)
    {
        float stretch = it->second;
        xmlInfo->Set("Stretch" + String(it->first), stretch);
    }
}
