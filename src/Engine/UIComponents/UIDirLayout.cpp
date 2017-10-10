#include "Bang/UIDirLayout.h"

#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

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

    Vector2i layoutRectSize = rt->GetScreenSpaceRectPx().GetSize();
    layoutRectSize -= GetPaddingSize();

    Vector2i availableSpace = layoutRectSize;
    availableSpace -= GetTotalSpacing();

    Array<Vector2i> childrenRTSizes(gameObject->GetChildren().Size(),
                                    Vector2i::Zero);

    FillChildrenMinSizes      (layoutRectSize, &childrenRTSizes, &availableSpace);
    FillChildrenPreferredSizes(layoutRectSize, &childrenRTSizes, &availableSpace);
    FillChildrenFlexibleSizes (layoutRectSize, &childrenRTSizes, &availableSpace);

    // Apply actual calculation to RectTransforms Margins
    uint i = 0;
    Vector2i marginAccum (GetPaddingLeft(), GetPaddingTop());
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i spacing = (i > 0) ? (GetDir() * GetSpacing()) : Vector2i::Zero;
        marginAccum += spacing;

        const Vector2i& childRTSize = childrenRTSizes[i];
        RectTransform *crt = child->GetComponent<RectTransform>();
        ApplyLayoutToChildRectTransform(layoutRectSize, crt, marginAccum,
                                        childRTSize);
        marginAccum += childRTSize;
        ++i;
    }
}

void UIDirLayout::ApplyLayoutToChildRectTransform(const Vector2i &layoutRectSize,
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
                crt->AddMarginLeft( (layoutRectSize.x - childRTSize.x) / 2);
            }
            else if (hAlign == HorizontalAlignment::Right)
            {
                crt->AddMarginLeft( (layoutRectSize.x - childRTSize.x) );
            }
            crt->SetMarginRight( -(crt->GetMarginLeft() + childRTSize.x) );
        }
        else
        {
            crt->SetMarginRight( -(GetPaddingLeft() + layoutRectSize.x) );
        }
        crt->SetMarginTop(  position.y );
        crt->SetMarginBot( -(crt->GetMarginTop() + childRTSize.y) );
    }
    else // HORIZONTAL
    {
        crt->SetMarginTop( GetPaddingTop() );
        if (GetChildrenVerticalStretch() == Stretch::None)
        {
            VerticalAlignment vAlign = GetChildrenVerticalAlignment();
            if (vAlign == VerticalAlignment::Center)
            {
                crt->AddMarginTop( (layoutRectSize.y - childRTSize.y) / 2);
            }
            else if (vAlign == VerticalAlignment::Bot)
            {
                crt->AddMarginTop( (layoutRectSize.y - childRTSize.y) );
            }
            crt->SetMarginBot( -(crt->GetMarginTop() + childRTSize.y) );
        }
        else
        {
            crt->SetMarginBot( -(GetPaddingTop() + layoutRectSize.y) );
        }
        crt->SetMarginLeft( position.x );
        crt->SetMarginRight( -(crt->GetMarginLeft() + childRTSize.x) );
    }
}

void UIDirLayout::FillChildrenMinSizes(const Vector2i &layoutRectSize,
                                       Array<Vector2i> *childrenRTSizes,
                                       Vector2i *availableSpace)
{
    uint i = 0;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childRTSize = UILayoutManager::GetMinSize(child);
        if (m_vertical)
        {
            availableSpace->y -= childRTSize.y;
            childRTSize.x = Math::Min(childRTSize.x, layoutRectSize.x);
        }
        else
        {
            availableSpace->x -= childRTSize.x;
            childRTSize.y = Math::Min(childRTSize.y, layoutRectSize.y);
        }

        (*childrenRTSizes)[i] = childRTSize;
        ++i;
    }
}

void UIDirLayout::FillChildrenPreferredSizes(const Vector2i &layoutRectSize,
                                             Array<Vector2i> *childrenRTSizes,
                                             Vector2i *availableSpace)
{
    uint i = 0;
    Vector2i totalPrefPxToAdd = Vector2i::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i minChildSize = (*childrenRTSizes)[i];
        Vector2i pxToAdd = UILayoutManager::GetPreferredSize(child) - minChildSize;
        pxToAdd = Vector2i::Max(pxToAdd, Vector2i::Zero);
        totalPrefPxToAdd += pxToAdd;
        ++i;
    }
    totalPrefPxToAdd = Vector2i::Max(totalPrefPxToAdd, Vector2i::One);

    // Populate with new children sizes
    i = 0;
    Array<Vector2i> newChildRTSizes;
    Array<Vector2i> childPreferredSizes;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i minChildSize = (*childrenRTSizes)[i];
        Vector2i childPrefSize = UILayoutManager::GetPreferredSize(child);
        Vector2i childPrefPxToAdd = (childPrefSize - minChildSize);
        childPrefPxToAdd = Vector2i::Max(childPrefPxToAdd, Vector2i::Zero);
        Vector2  sizeProportion (Vector2(childPrefPxToAdd) / Vector2(totalPrefPxToAdd));
        Vector2i prefAvailPxToAdd(
                   Vector2::Ceil(sizeProportion * Vector2(*availableSpace)));
        prefAvailPxToAdd = Vector2i::Min(prefAvailPxToAdd, childPrefPxToAdd);
        Vector2i childRTSize = Vector2i::Max(minChildSize,
                                             minChildSize + prefAvailPxToAdd);

        newChildRTSizes.PushBack(childRTSize);
        childPreferredSizes.PushBack(childPrefSize);
        ++i;
    }

    // Apply children sizes populating the final array
    for (i = 0; i < newChildRTSizes.Size(); ++i)
    {
        // Apply children sizes populating the final array
        Vector2i newChildRTSize = newChildRTSizes[i];
        Vector2i minChildSize = (*childrenRTSizes)[i];
        if (m_vertical)
        {
            availableSpace->y -= (newChildRTSize.y - minChildSize.y);
            newChildRTSize.x = Math::Min(newChildRTSize.x, layoutRectSize.x);
        }
        else
        {
            availableSpace->x -= (newChildRTSize.x - minChildSize.x);
            newChildRTSize.y = Math::Min(newChildRTSize.y, layoutRectSize.y);
        }

        newChildRTSizes[i] = newChildRTSize;
    }

    // Make up for precision problems (make sure we havent missed any pixel)
    for (i = 0; i < newChildRTSizes.Size(); ++i)
    {
        Vector2i childPrefSize = childPreferredSizes[i];
        Vector2i newChildRTSize = newChildRTSizes[i];
        Vector2i prefPxToFill = (childPrefSize - newChildRTSize);
        if (m_vertical)
        {
            if(prefPxToFill.y > 0)
            {
                newChildRTSize.y  += prefPxToFill.y;
                availableSpace->y -= prefPxToFill.y;
            }
        }
        else
        {
            if(prefPxToFill.x > 0)
            {
                newChildRTSize.x  += prefPxToFill.x;
                availableSpace->x -= prefPxToFill.x;
            }
        }

        (*childrenRTSizes)[i] = newChildRTSize;
    }

    *childrenRTSizes = newChildRTSizes;
}

void UIDirLayout::FillChildrenFlexibleSizes(const Vector2i &layoutRectSize,
                                            Array<Vector2i> *childrenRTSizes,
                                            Vector2i *availableSpace)
{
    Vector2 totalChildrenFlexSize = Vector2::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        totalChildrenFlexSize += UILayoutManager::GetFlexibleSize(child);
    }
    totalChildrenFlexSize = Vector2::Max(totalChildrenFlexSize, Vector2(0.0001));

    // Populate with new children sizes
    uint i = 0;
    Array<Vector2i> newChildRTSizes;
    Array<Vector2>  childFlexibleSizes;
    Vector2 originalAvailableSpace( *availableSpace );
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i prefChildSize = (*childrenRTSizes)[i];
        Vector2  childFlexSize = UILayoutManager::GetFlexibleSize(child);
        Vector2  sizeProportion (childFlexSize / totalChildrenFlexSize);
        Vector2i flexAvailPxToAdd (
                    Vector2::Round(sizeProportion * originalAvailableSpace) );
        Vector2i childRTSize = Vector2i::Max(prefChildSize,
                                             prefChildSize + flexAvailPxToAdd);
        if (m_vertical)
        {
            childRTSize.x = Math::Min(childRTSize.x, layoutRectSize.x);
            availableSpace->y -= flexAvailPxToAdd.y;
        }
        else
        {
            childRTSize.y = Math::Min(childRTSize.y, layoutRectSize.y);
            availableSpace->x -= flexAvailPxToAdd.x;
        }

        newChildRTSizes.PushBack(childRTSize);
        childFlexibleSizes.PushBack(childFlexSize);
        ++i;
    }

    // Make up for precision problems (make sure we havent missed any pixel)
    for (i = 0; i < newChildRTSizes.Size(); ++i)
    {
        if (availableSpace->x == 0 && availableSpace->y == 0) { break; }

        Vector2 childFlexSize = childFlexibleSizes[i];
        if (m_vertical && childFlexSize.y > 0.0f && availableSpace->y != 0)
        {
            newChildRTSizes[i].y += availableSpace->y;
            availableSpace->y = 0;
        }

        if (!m_vertical && childFlexSize.x > 0.0f && availableSpace->x != 0)
        {
            newChildRTSizes[i].x += availableSpace->x;
            availableSpace->x = 0;
        }
    }

    *childrenRTSizes = newChildRTSizes;
}

Vector2i UIDirLayout::_GetMinSize() const
{
    return GetSize(LayoutSizeType::Min);
}

Vector2i UIDirLayout::_GetPreferredSize() const
{
    return GetSize(LayoutSizeType::Preferred);
}

Vector2i UIDirLayout::GetSize(LayoutSizeType sizeType) const
{
    Vector2i totalSize = Vector2i::Zero;
    for (GameObject *child : gameObject->GetChildren())
    {
        Vector2i childSize (UILayoutManager::GetSize(child, sizeType));
        if (m_vertical)
        {
            totalSize.x = Math::Max(totalSize.x, childSize.x);
            totalSize.y += childSize.y;
        }
        else
        {
            totalSize.x += childSize.x;
            totalSize.y = Math::Max(totalSize.y, childSize.y);
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
