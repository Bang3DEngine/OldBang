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

void UIDirLayout::_ApplyLayout()
{
    RectTransform *rt = GetGameObject()->GetComponent<RectTransform>(); ENSURE(rt);
    auto children = UILayoutManager::GetLayoutableChildrenList(GetGameObject());

    // if (GetGameObject()->GetName() == "MenuBarItem") { Debug_Log(children); }

    Vector2i layoutRectSize = rt->GetScreenSpaceRectPx().GetSize();
    layoutRectSize -= GetPaddingSize();

    Vector2i availableSpace = layoutRectSize;
    availableSpace -= GetTotalSpacing(children);

    Array<Vector2i> childrenRTSizes(children.Size(), Vector2i::Zero);

    FillChildrenMinSizes(layoutRectSize, children,
                         &childrenRTSizes, &availableSpace);
    FillChildrenPreferredSizes(layoutRectSize, children,
                               &childrenRTSizes, &availableSpace);
    FillChildrenFlexibleSizes(layoutRectSize, children,
                              &childrenRTSizes, &availableSpace);

    // Apply actual calculation to RectTransforms Margins
    uint i = 0;
    Vector2i marginAccum (GetPaddingLeft(), GetPaddingTop());
    for (GameObject *child : children)
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

    if (GetAxis() == Axis::Vertical)
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
    else // Axis::Horizontal
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
        Vector2d  sizeProportion (Vector2d(childPrefPxToAdd) /
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

Vector2i UIDirLayout::_GetMinSize() const
{
    return GetSize(UILayoutManager::GetLayoutableChildrenList( GetGameObject() ),
                   LayoutSizeType::Min);
}

Vector2i UIDirLayout::_GetPreferredSize() const
{
    return GetSize(UILayoutManager::GetLayoutableChildrenList( GetGameObject() ),
                   LayoutSizeType::Preferred);
}

Vector2i UIDirLayout::GetSize(const List<GameObject*> &children,
                              LayoutSizeType sizeType) const
{
    Vector2i totalSize = Vector2i::Zero;
    for (GameObject *child : children)
    {
        Vector2i childSize (UILayoutManager::GetSize(child, sizeType));
        if (GetAxis() == Axis::Vertical)
        {
            totalSize.x = Math::Max(totalSize.x, childSize.x);
            totalSize.y += childSize.y;
        }
        else // Axis::Horizontal
        {
            totalSize.x += childSize.x;
            totalSize.y = Math::Max(totalSize.y, childSize.y);
        }
    }
    return totalSize + GetTotalSpacing(children) + GetPaddingSize();
}

void UIDirLayout::SetSpacing(int spacingPx) { m_spacingPx = spacingPx; }

int UIDirLayout::GetSpacing() const { return m_spacingPx; }

Axis UIDirLayout::GetAxis() const
{
    return m_axis;
}

Vector2i UIDirLayout::GetDir() const
{
    return Vector2i::FromAxis(m_axis);
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
