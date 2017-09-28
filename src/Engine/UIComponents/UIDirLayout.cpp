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
    layoutRect = Recti(layoutRect.GetMin() + GetPaddingLeftBot(),
                       layoutRect.GetMax() - GetPaddingRightTop());

    Vector2i availableSize = layoutRect.GetSize();
    availableSize -= GetTotalSpacing();

    Array<Vector2i> childrenRTSizes(gameObject->GetChildren().Size(),
                                    Vector2i::Zero);

    FillChildrenSizes(layoutRect, &childrenRTSizes, &availableSize,
                      LayoutSizeType::Min);
    FillChildrenSizes(layoutRect, &childrenRTSizes, &availableSize,
                      LayoutSizeType::Preferred);
    FillChildrenSizes(layoutRect, &childrenRTSizes, &availableSize,
                      LayoutSizeType::Flexible);

    // Apply actual calculation to RectTransforms Margins
    uint i = 0;
    Vector2i marginAccum = Vector2i::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i spacing = (i > 0) ? (GetDir() * GetSpacing()) : Vector2i::Zero;
        marginAccum += spacing;

        const Vector2i& childRTSize = childrenRTSizes[i];
        RectTransform *crt = child->GetComponent<RectTransform>();
        ApplyLayoutToChildRectTransform(layoutRect, crt, marginAccum, childRTSize);
        marginAccum += childRTSize;
        ++i;
    }
}

void UIDirLayout::ApplyLayoutToChildRectTransform(const Recti &layoutRect,
                                                  RectTransform *crt,
                                                  const Vector2i &position,
                                                  const Vector2i &childRTSize)
{
    ENSURE(crt);
    crt->SetAnchors( Vector2(-1, 1) );
    if (m_vertical) // VERTICAL
    {
        crt->SetMarginLeft( GetPaddingLeft() );
        if (GetChildrenHorizontalStretch() == Stretch::None)
        {
            HorizontalAlignment hAlign = GetChildrenHorizontalAlignment();
            if (hAlign == HorizontalAlignment::Center)
            {
                crt->AddMarginLeft( (layoutRect.GetWidth() - childRTSize.x) / 2);
            }
            else if (hAlign == HorizontalAlignment::Right)
            {
                crt->AddMarginLeft( (layoutRect.GetWidth() - childRTSize.x) );
            }
            crt->SetMarginRight( -(crt->GetMarginLeft() + childRTSize.x) );
        }
        else
        {
            crt->SetMarginRight( -(GetPaddingLeft() + layoutRect.GetWidth()) );
        }
        crt->SetMarginTop( position.y );
        crt->SetMarginBot( -(position.y + childRTSize.y) );
    }
    else // HORIZONTAL
    {
        crt->SetMarginTop( GetPaddingTop() );
        if (GetChildrenVerticalStretch() == Stretch::None)
        {
            VerticalAlignment vAlign = GetChildrenVerticalAlignment();
            if (vAlign == VerticalAlignment::Center)
            {
                crt->AddMarginTop( (layoutRect.GetHeight() - childRTSize.y) / 2);
            }
            else if (vAlign == VerticalAlignment::Bot)
            {
                crt->AddMarginTop( (layoutRect.GetHeight() - childRTSize.y) );
            }
            crt->SetMarginBot( -(crt->GetMarginTop() + childRTSize.y) );
        }
        else
        {
            crt->SetMarginBot( -(GetPaddingTop() + layoutRect.GetHeight()) );
        }
        crt->SetMarginLeft( position.x );
        crt->SetMarginRight( -(position.x + childRTSize.x) );
    }
}

void UIDirLayout::FillChildrenSizes(const Recti &layoutRect,
                                    Array<Vector2i> *childrenRTSizes,
                                    Vector2i *availableSize,
                                    LayoutSizeType sizeType)
{
    uint i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i originalChildRTSize = childrenRTSizes->At(i);
        Vector2i childRTSize = originalChildRTSize;
        List<ILayoutElement*> cles = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *cle : cles)
        {
            Vector2i pxToAdd(cle->GetTotalSize(sizeType) - originalChildRTSize);
            if (sizeType != LayoutSizeType::Min)
            {
                pxToAdd = Vector2i::Min(pxToAdd, *availableSize);
            }
            pxToAdd = Vector2i::Max(pxToAdd, Vector2i::Zero);
            childRTSize = Vector2i::Max(childRTSize, originalChildRTSize + pxToAdd);
        }

        if (m_vertical)
        {
            availableSize->y -= (childRTSize.y - originalChildRTSize.y);
            childRTSize.x = Math::Min(childRTSize.x, layoutRect.GetSize().x);
        }
        else
        {
            availableSize->x -= (childRTSize.x - originalChildRTSize.x);
            childRTSize.y = Math::Min(childRTSize.y, layoutRect.GetSize().y);
        }

        (*childrenRTSizes)[i] = childRTSize;
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
    return totalSize + GetTotalSpacing() + GetPaddingSize();
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
