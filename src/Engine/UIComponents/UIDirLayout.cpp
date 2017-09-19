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
    const Vector2i spacing = GetDir() * GetSpacing();
    return spacing * SCAST<int>(gameObject->GetChildren().Size() - 1);
}

void UIDirLayout::ApplyLayout()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);
    Recti layoutRect = rt->GetScreenSpaceRectPx();

    Vector2i availableSize = layoutRect.GetSize();
    availableSize -= GetTotalSpacing();

    Array<Vector2i> childrenSizes(gameObject->GetChildren().Size(),
                                  Vector2i::Zero);

    FillChildrenSizes(&childrenSizes, &availableSize, LayoutSizeType::Min);
    FillChildrenSizes(&childrenSizes, &availableSize, LayoutSizeType::Preferred);
    FillChildrenSizes(&childrenSizes, &availableSize, LayoutSizeType::Flexible);

    // Apply actual calculation to RectTransforms Margins
    uint i = 0;
    Vector2i marginAccum = Vector2i::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i spacing = (i > 0) ? (GetDir() * GetSpacing()) : Vector2i::Zero;
        marginAccum += spacing;

        const Vector2i& childSize = childrenSizes[i];
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (m_vertical)
        {
            crt->SetAnchorMinY(1.0f);
            crt->SetAnchorMaxY(1.0f);
            crt->SetMarginTop(   marginAccum.y );
            crt->SetMarginBot( -(marginAccum.y + childSize.y) );
        }
        else
        {
            crt->SetAnchorMinX(-1.0f);
            crt->SetAnchorMaxX(-1.0f);
            crt->SetMarginLeft (   marginAccum.x );
            crt->SetMarginRight( -(marginAccum.x + childSize.x) );
        }

        marginAccum += childSize;
        ++i;
    }
}

void UIDirLayout::FillChildrenSizes(Array<Vector2i> *childrenSizes,
                                    Vector2i *availableSize,
                                    LayoutSizeType sizeType)
{
    uint i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childSize = childrenSizes->At(i);
        List<ILayoutElement*> cles = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *cle : cles)
        {
            Vector2i pxToAdd(cle->GetTotalSize(sizeType) - childSize);
            pxToAdd = Vector2i::Min(pxToAdd, *availableSize);
            pxToAdd = Vector2i::Max(pxToAdd, Vector2i::Zero);
            childSize = Vector2i::Max(childSize, childSize + pxToAdd);
        }
        *availableSize -= (childSize - childrenSizes->At(i));
        (*childrenSizes)[i] = childSize;
        ++i;
    }
}

Vector2i UIDirLayout::CalculateTotalMinSize() const
{
    return CalculateTotalSize(LayoutSizeType::Min);
}

Vector2i UIDirLayout::CalculateTotalPreferredSize() const
{
    return CalculateTotalSize(LayoutSizeType::Preferred);
}

Vector2i UIDirLayout::CalculateTotalFlexiblePxSize() const
{
    return CalculateTotalSize(LayoutSizeType::Flexible);
}

Vector2i UIDirLayout::CalculateTotalSize(LayoutSizeType sizeType) const
{
    Vector2i totalSize = Vector2i::Zero;
    List<ILayoutElement*> cles =
            gameObject->GetComponentsInChildrenOnly<ILayoutElement>(false);
    for (ILayoutElement *cle : cles)
    {
        Vector2i childTotalSize = cle->GetTotalSize(sizeType);
        if (m_vertical)
        {
            totalSize.x = Math::Max(totalSize.x, childTotalSize.x);
            totalSize.y += childTotalSize.y;
        }
        else
        {
            totalSize.x += childTotalSize.x;
            totalSize.y = Math::Max(totalSize.y, childTotalSize.y);
        }
    }
    return totalSize + GetTotalSpacing();
}

void UIDirLayout::SetSpacing(int spacingPx) { m_spacingPx = spacingPx; }

int UIDirLayout::GetSpacing() const { return m_spacingPx; }

Vector2i UIDirLayout::GetDir() const
{
    return m_vertical ? Vector2i::Up : Vector2i::Right;
}

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
