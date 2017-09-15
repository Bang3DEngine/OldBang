#include "Bang/UIDirLayout.h"

#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/XMLNode.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"

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

    Array<Vector2i> childrenSizes(numChildren, Vector2i::Zero);

    // Apply minSizes
    int i = 0;
    Vector2i pxAvailableSize = layoutRect.GetSize();
    for (GameObject *child : gameObject->GetChildren())
    {
        UILayoutElement *cle = child->GetComponent<UILayoutElement>();
        if (!cle) { continue; }

        childrenSizes[i]  = cle->GetMinSize();
        pxAvailableSize  -= cle->GetMinSize();
        ++i;
    }

    // Apply preferredSizes
    i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        UILayoutElement *cle = child->GetComponent<UILayoutElement>();
        if (!cle) { continue; }

        Vector2i pxToAdd(cle->GetPreferredSize() - cle->GetMinSize());
        pxToAdd = Vector2i::Clamp(pxToAdd, Vector2i::Zero, pxAvailableSize);

        childrenSizes[i] += pxToAdd;
        pxAvailableSize  -= pxToAdd;
        ++i;
    }

    // Apply flexibleSizes
    i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        UILayoutElement *cle = child->GetComponent<UILayoutElement>();
        if (!cle) { continue; }

        Vector2i pxToAdd(Vector2::Round(cle->GetFlexibleSize() *
                                        Vector2(cle->GetPreferredSize())));
        pxToAdd = Vector2i::Clamp(pxToAdd, Vector2i::Zero, pxAvailableSize);

        childrenSizes[i] += pxToAdd;
        pxAvailableSize  -= pxToAdd;
        ++i;
    }

    // Apply actual calculation to RectTransforms Margins
    i = 0;
    Vector2i marginAccum = Vector2i::Zero;
    const Vector2i dir = (m_vertical ? Vector2i::Up : Vector2i::Right);
    for (GameObject *child : gameObject->GetChildren())
    {
        const Vector2i& childSize = childrenSizes[i];
        const Vector2i spacing = dir * (i > 0 ? GetSpacing() : 0);
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (m_vertical)
        {
            crt->SetMarginTop  (   spacing.y + marginAccum.y );
            crt->SetMarginBot  ( -(spacing.y + marginAccum.y + childSize.y) );
            crt->SetAnchorMin( Vector2(crt->GetAnchorMin().x, -1.0f) );
            crt->SetAnchorMax( Vector2(crt->GetAnchorMax().x, -1.0f) );
        }
        else
        {
            crt->SetMarginLeft (   spacing.x + marginAccum.x );
            crt->SetMarginRight( -(spacing.x + marginAccum.x + childSize.x) );
            crt->SetAnchorMin( Vector2(-1.0f, crt->GetAnchorMin().y) );
            crt->SetAnchorMax( Vector2(-1.0f, crt->GetAnchorMax().y) );
        }

        marginAccum += (Vector2i(spacing) + childSize);
        ++i;
    }
}

void UIDirLayout::SetSpacing(int spacingPx) { m_spacingPx = spacingPx; }

int UIDirLayout::GetSpacing() const { return m_spacingPx; }

void UIDirLayout::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("SpacingPx"))
    { SetSpacing( xmlInfo.Get<int>("SpacingPx") ); }
}

void UIDirLayout::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("SpacingPx", m_spacingPx);
}
