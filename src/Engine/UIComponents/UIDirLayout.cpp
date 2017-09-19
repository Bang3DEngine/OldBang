#include "Bang/UIDirLayout.h"

#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/XMLNode.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ILayoutElement.h"

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

Vector2i UIDirLayout::GetTotalSpacing() const
{
    const Vector2i dir = (m_vertical ? Vector2i::Up : Vector2i::Right);
    const Vector2i spacing = dir * GetSpacing();
    return spacing * SCAST<int>(gameObject->GetChildren().Size() - 1);
}

void UIDirLayout::ApplyLayoutToChildren()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);
    Recti layoutRect = rt->GetScreenSpaceRectPx();

    Vector2i pxAvailableSize = layoutRect.GetSize();
    pxAvailableSize -= GetTotalSpacing();

    Array<Vector2i> childrenSizes;

    // Apply minSizes
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childSize = Vector2i::Zero;
        List<ILayoutElement*> cles = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *cle : cles)
        {
            Vector2i pxToAdd = cle->GetTotalMinSize();
            childSize = Vector2i::Max(childSize, pxToAdd);
        }
        pxAvailableSize -= childSize;
        childrenSizes.PushBack(childSize);
    }
    // Debug_Peek(pxAvailableSize);
    // Debug_Log("MinSizes: " << childrenSizes);

    // Apply preferredSizes
    uint i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childSize = childrenSizes[i];
        List<ILayoutElement*> cles = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *cle : cles)
        {
            Vector2i pxToAdd(cle->GetTotalPreferredSize() - cle->GetTotalMinSize());
            pxToAdd = Vector2i::Min(pxToAdd, pxAvailableSize);
            pxToAdd = Vector2i::Max(pxToAdd, Vector2i::Zero);
            childSize = Vector2i::Max(childSize, childSize + pxToAdd);
        }
        pxAvailableSize -= (childSize - childrenSizes[i]);
        childrenSizes[i] = childSize;
        ++i;
    }
    // Debug_Log("pxAvailableSize: " << pxAvailableSize);
    // Debug_Log("PreferredSizes: " << childrenSizes);

    // Apply flexibleSizes
    i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childSize = childrenSizes[i];
        List<ILayoutElement*> cles = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *cle : cles)
        {
            Vector2i pxToAdd(cle->GetTotalFlexiblePxSize() -
                             cle->GetTotalPreferredSize());
            pxToAdd = Vector2i::Min(pxToAdd, pxAvailableSize);
            pxToAdd = Vector2i::Max(pxToAdd, Vector2i::Zero);
            childSize = Vector2i::Max(childSize, childSize + pxToAdd);
        }
        pxAvailableSize -= (childSize - childrenSizes[i]);
        childrenSizes[i] = childSize;
        ++i;
    }

    // Apply actual calculation to RectTransforms Margins
    i = 0;
    Vector2i marginAccum = Vector2i::Zero;
    const Vector2i dir = (m_vertical ? Vector2i::Up : Vector2i::Right);
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i spacing = (i > 0) ? (dir * GetSpacing()) : Vector2i::Zero;
        marginAccum += spacing;

        const Vector2i& childSize = childrenSizes[i];
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (m_vertical)
        {
            crt->SetMarginTop  ( marginAccum.y );
            crt->SetMarginBot  ( -(marginAccum.y + childSize.y) );
            crt->SetAnchorMin( Vector2(crt->GetAnchorMin().x, 1.0f) );
            crt->SetAnchorMax( Vector2(crt->GetAnchorMax().x, 1.0f) );
        }
        else
        {
            crt->SetMarginLeft (   marginAccum.x );
            crt->SetMarginRight( -(marginAccum.x + childSize.x) );
            crt->SetAnchorMin( Vector2(-1.0f, crt->GetAnchorMin().y) );
            crt->SetAnchorMax( Vector2(-1.0f, crt->GetAnchorMax().y) );
        }

        // Debug_Peek(crt->GetScreenSpaceRectPx());
        marginAccum += childSize;
        ++i;
    }
    // Debug_Log("================================");
}

Vector2i UIDirLayout::CalculateTotalMinSize() const
{
    Vector2i totalMinSize = Vector2i::Zero;
    List<ILayoutElement*> cles =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    for (ILayoutElement *cle : cles)
    {
        Vector2i childTotalMinSize = cle->GetTotalMinSize();
        if (m_vertical)
        {
            totalMinSize.x = Math::Max(totalMinSize.x, childTotalMinSize.x);
            totalMinSize.y += childTotalMinSize.y;
        }
        else
        {
            totalMinSize.x += childTotalMinSize.x;
            totalMinSize.y = Math::Max(totalMinSize.y, childTotalMinSize.y);
        }
    }
    return totalMinSize + GetTotalSpacing();
}

Vector2i UIDirLayout::CalculateTotalPreferredSize() const
{
    Vector2i totalPrefSize = Vector2i::Zero;
    List<ILayoutElement*> cles =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    for (ILayoutElement *cle : cles)
    {
        Vector2i cTotalPrefSize = cle->GetTotalPreferredSize();
        if (m_vertical)
        {
            totalPrefSize.x = Math::Max(totalPrefSize.x, cTotalPrefSize.x);
            totalPrefSize.y += cTotalPrefSize.y;
        }
        else
        {
            totalPrefSize.x += cTotalPrefSize.x;
            totalPrefSize.y = Math::Max(totalPrefSize.y, cTotalPrefSize.y);
        }
    }
    return totalPrefSize + GetTotalSpacing();
}

Vector2i UIDirLayout::CalculateTotalFlexiblePxSize() const
{
    Vector2i totalFlexSize = Vector2i::Zero;
    List<ILayoutElement*> cles =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    for (ILayoutElement *cle : cles)
    {
        Vector2i cTotalFlexSize = cle->GetTotalFlexiblePxSize();
        if (m_vertical)
        {
            totalFlexSize.x = Math::Max(totalFlexSize.x, cTotalFlexSize.x);
            totalFlexSize.y += cTotalFlexSize.y;
        }
        else
        {
            totalFlexSize.x += cTotalFlexSize.x;
            totalFlexSize.y = Math::Max(totalFlexSize.y, cTotalFlexSize.y);
        }
    }
    return totalFlexSize + GetTotalSpacing();
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
