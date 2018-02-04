#include "Bang/UIDirLayout.h"

#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UIDirLayout::UIDirLayout() : UIDirLayout(Axis::Vertical)
{
}

UIDirLayout::UIDirLayout(Axis axis) : m_axis(axis)
{
}

UIDirLayout::~UIDirLayout()
{
}

Vector2i UIDirLayout::GetTotalSpacing(const List<GameObject*> &children) const
{
    const Vector2i spacing = GetDir() * GetSpacing();
    return spacing * SCAST<int>(children.Size() - 1);
}

void UIDirLayout::ApplyLayout(Axis axis)
{
    RectTransform *rt = GetGameObject()->GetRectTransform();
    if (!rt) { return; }
    List<GameObject*> children =
                    UILayoutManager::GetLayoutableChildrenList(GetGameObject());

    Vector2i layoutRectSize( Vector2::Round(rt->GetViewportRect().GetSize()) );
    Vector2i paddedLayoutRectSize = layoutRectSize - GetPaddingSize();

    Array<Vector2i> childrenRTSizes(children.Size(), Vector2i::Zero);
    {
        Vector2i availableSpace = paddedLayoutRectSize - GetTotalSpacing(children);

        FillChildrenMinSizes(paddedLayoutRectSize, children,
                             &childrenRTSizes, &availableSpace);
        FillChildrenPreferredSizes(paddedLayoutRectSize, children,
                                   &childrenRTSizes, &availableSpace);
        FillChildrenFlexibleSizes(paddedLayoutRectSize, children,
                                  &childrenRTSizes, &availableSpace);
        ApplyStretches(paddedLayoutRectSize, &childrenRTSizes);
    }

    // Apply actual calculation to RectTransforms Margins
    uint i = 0;
    Vector2i currentTopLeft(GetPaddingLeft(), GetPaddingTop());
    for (GameObject *child : children)
    {
        Vector2i spacing = (i > 0) ? (GetDir() * GetSpacing()) : Vector2i::Zero;
        currentTopLeft += spacing;

        Vector2i childRTSize = childrenRTSizes[i];
        RectTransform *crt = child->GetRectTransform();

        ApplyLayoutToChildRectTransform(axis, layoutRectSize, crt,
                                        currentTopLeft, childRTSize);
        currentTopLeft += childRTSize;
        ++i;
    }
}

void UIDirLayout::ApplyLayoutToChildRectTransform(Axis rebuildPassAxis,
                                                  const Vector2i &layoutRectSize,
                                                  RectTransform *crt,
                                                  const Vector2i &position,
                                                  const Vector2i &childRTSize)
{
    if (!crt) { return; }
    crt->SetAnchors( Vector2(-1, 1) );

    Vector2i paddedLayoutRectSize = layoutRectSize - GetPaddingSize();
    if (GetAxis() == Axis::Vertical)
    {
        if (rebuildPassAxis == Axis::Horizontal)
        {
            crt->SetMarginLeft( GetPaddingLeft() );

            HorizontalAlignment hAlign = GetChildrenHorizontalAlignment();
            if (hAlign == HorizontalAlignment::Center)
            {
                crt->AddMarginLeft( (paddedLayoutRectSize.x - childRTSize.x) / 2);
            }
            else if (hAlign == HorizontalAlignment::Right)
            {
                crt->AddMarginLeft( (paddedLayoutRectSize.x - childRTSize.x) );
            }
            crt->SetMarginRight( -(crt->GetMarginLeft() + childRTSize.x) );
        }
        else // Axis::Vertical
        {
            crt->SetMarginTop(  position.y );
            crt->SetMarginBot( -(crt->GetMarginTop() + childRTSize.y) );
        }
    }
    else // Axis::Horizontal
    {
        if (rebuildPassAxis == Axis::Vertical)
        {
            crt->SetMarginTop( GetPaddingTop() );

            VerticalAlignment vAlign = GetChildrenVerticalAlignment();
            if (vAlign == VerticalAlignment::Center)
            {
                crt->AddMarginTop( (paddedLayoutRectSize.y - childRTSize.y) / 2);
            }
            else if (vAlign == VerticalAlignment::Bot)
            {
                crt->AddMarginTop( (paddedLayoutRectSize.y - childRTSize.y) );
            }
            crt->SetMarginBot( -(crt->GetMarginTop() + childRTSize.y) );
        }
        else // Axis::Horizontal
        {
            crt->SetMarginLeft( position.x );
            crt->SetMarginRight( -(crt->GetMarginLeft() + childRTSize.x) );
        }
    }
}

void UIDirLayout::FillChildrenMinSizes(const Vector2i &layoutRectSize,
                                       const List<GameObject*> &children,
                                       Array<Vector2i> *childrenRTSizes,
                                       Vector2i *availableSpace)
{
    uint i = 0;
    for (GameObject *child : children)
    {
        Vector2i childRTSize = UILayoutManager::GetMinSize(child);
        if (GetAxis() == Axis::Vertical)
        {
            availableSpace->y -= childRTSize.y;
            childRTSize.x = Math::Min(childRTSize.x, layoutRectSize.x);
        }
        else // Axis::Horizontal
        {
            availableSpace->x -= childRTSize.x;
            childRTSize.y = Math::Min(childRTSize.y, layoutRectSize.y);
        }

        (*childrenRTSizes)[i] = childRTSize;
        ++i;
    }
}

void UIDirLayout::FillChildrenPreferredSizes(const Vector2i &layoutRectSize,
                                             const List<GameObject*> &children,
                                             Array<Vector2i> *childrenRTSizes,
                                             Vector2i *availableSpace)
{
    uint i = 0;
    Vector2i totalPrefPxToAdd = Vector2i::Zero;
    for (GameObject *child : children)
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
    for (GameObject *child : children)
    {
        Vector2i minChildSize = (*childrenRTSizes)[i];
        Vector2i childPrefSize = UILayoutManager::GetPreferredSize(child);
        Vector2i childPrefPxToAdd = (childPrefSize - minChildSize);
        childPrefPxToAdd = Vector2i::Max(childPrefPxToAdd, Vector2i::Zero);
        Vector2d sizeProportion (Vector2d(childPrefPxToAdd) /
                                 Vector2d(totalPrefPxToAdd));
        Vector2i prefAvailPxToAdd (sizeProportion * Vector2d(*availableSpace));
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
        if (GetAxis() == Axis::Vertical)
        {
            availableSpace->y -= (newChildRTSize.y - minChildSize.y);
            newChildRTSize.x = Math::Min(newChildRTSize.x, layoutRectSize.x);
        }
        else // Axis::Horizontal
        {
            availableSpace->x -= (newChildRTSize.x - minChildSize.x);
            newChildRTSize.y = Math::Min(newChildRTSize.y, layoutRectSize.y);
        }

        newChildRTSizes[i] = newChildRTSize;
    }

    *childrenRTSizes = newChildRTSizes;
}

void UIDirLayout::FillChildrenFlexibleSizes(const Vector2i &layoutRectSize,
                                            const List<GameObject*> &children,
                                            Array<Vector2i> *childrenRTSizes,
                                            Vector2i *availableSpace)
{
    Vector2d totalChildrenFlexSize = Vector2d::Zero;
    for (GameObject *child : children)
    {
        totalChildrenFlexSize += Vector2d(UILayoutManager::GetFlexibleSize(child));
    }
    totalChildrenFlexSize = Vector2d::Max(totalChildrenFlexSize, Vector2d(0.0001));

    // Populate with new children sizes
    uint i = 0;
    Array<Vector2i> newChildRTSizes;
    Array<Vector2d> childFlexibleSizes;
    Vector2d originalAvailableSpace( *availableSpace );
    for (GameObject *child : children)
    {
        Vector2i prefChildSize    ( (*childrenRTSizes)[i] );
        Vector2d childFlexSize    ( UILayoutManager::GetFlexibleSize(child) );
        Vector2d sizeProportion   (childFlexSize / totalChildrenFlexSize);
        Vector2i flexAvailPxToAdd (sizeProportion * originalAvailableSpace);
        Vector2i childRTSize = Vector2i::Max(prefChildSize,
                                             prefChildSize + flexAvailPxToAdd);
        if (GetAxis() == Axis::Vertical)
        {
            childRTSize.x = Math::Min(childRTSize.x, layoutRectSize.x);
            availableSpace->y -= flexAvailPxToAdd.y;
        }
        else // Axis::Horizontal
        {
            childRTSize.y = Math::Min(childRTSize.y, layoutRectSize.y);
            availableSpace->x -= flexAvailPxToAdd.x;
        }

        newChildRTSizes.PushBack(childRTSize);
        childFlexibleSizes.PushBack(childFlexSize);
        ++i;
    }

    *childrenRTSizes = newChildRTSizes;
}

void UIDirLayout::ApplyStretches(const Vector2i &layoutRectSize,
                                 Array<Vector2i> *childrenRTSizes)
{
    Array<Vector2i> newChildrenRTSizes;
    for (const Vector2i& childRTSize : *childrenRTSizes)
    {
        Vector2i newChildRTSize = childRTSize;
        if (GetChildrenHorizontalStretch() == Stretch::Full)
        {
            if (GetAxis() == Axis::Horizontal)
            { newChildRTSize.x = layoutRectSize.x / childrenRTSizes->Size(); }
            else { newChildRTSize.x = layoutRectSize.x; }
        }

        if (GetChildrenVerticalStretch() == Stretch::Full)
        {
            if (GetAxis() == Axis::Vertical)
            { newChildRTSize.y = layoutRectSize.y / childrenRTSizes->Size(); }
            else { newChildRTSize.y = layoutRectSize.y; }
        }

        newChildrenRTSizes.PushBack(newChildRTSize);
    }

    *childrenRTSizes = newChildrenRTSizes;
}

void UIDirLayout::CalculateLayout(Axis axis)
{
    Vector2i minSize  = Vector2i::Zero;
    Vector2i prefSize = Vector2i::Zero;
    List<GameObject*> children =
                UILayoutManager::GetLayoutableChildrenList( GetGameObject() );
    for (GameObject *child : children)
    {
        Vector2i cMinSize (UILayoutManager::GetSize(child, LayoutSizeType::Min));
        Vector2i cPrefSize(UILayoutManager::GetSize(child, LayoutSizeType::Preferred));
        if (GetAxis() == Axis::Vertical)
        {
            minSize.x  = Math::Max(minSize.x, cMinSize.x);
            prefSize.x = Math::Max(prefSize.x, cPrefSize.x);
            minSize.y  += cMinSize.y;
            prefSize.y += cPrefSize.y;
        }
        else // Axis::Horizontal
        {
            minSize.x  += cMinSize.x;
            prefSize.x += cPrefSize.x;
            minSize.y  = Math::Max(minSize.y, cMinSize.y);
            prefSize.y = Math::Max(prefSize.y, cPrefSize.y);
        }
    }

    Vector2i addedSize (GetTotalSpacing(children) + GetPaddingSize());
    minSize  += addedSize;
    prefSize += addedSize;

    SetCalculatedLayout(axis, minSize.GetAxis(axis), prefSize.GetAxis(axis));
}

Axis UIDirLayout::GetAxis() const { return m_axis; }
Vector2i UIDirLayout::GetDir() const { return Vector2i::FromAxis(m_axis); }
