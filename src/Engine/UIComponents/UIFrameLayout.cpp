#include "Bang/UIFrameLayout.h"

#include "Bang/Rect.h"
#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UIFrameLayout::UIFrameLayout()
{

}

UIFrameLayout::~UIFrameLayout()
{

}

void UIFrameLayout::ApplyLayout()
{
    RectTransform *layoutRT = gameObject->GetComponent<RectTransform>();
    ENSURE(layoutRT);
    Vector2i layoutRectSize = layoutRT->GetScreenSpaceRectPx().GetSize();
    layoutRectSize -= GetPaddingSize();
    Vector2i availablePx = layoutRectSize;

    for (GameObject *child : gameObject->GetChildren())
    {
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (crt && child->HasComponent<ILayoutElement>())
        {
            Vector2i childSize = UILayoutManager::GetMinSize(child);
            availablePx = Vector2i::Max(Vector2i::Zero, availablePx - childSize);

            Vector2i prefChildSize = UILayoutManager::GetPreferredSize(child);
            Vector2i pxToAdd = Vector2i::Max(Vector2i::Zero,
                                             prefChildSize - childSize);
            childSize += Vector2i::Min(availablePx, pxToAdd);

            if (UILayoutManager::GetFlexibleSize(child).x > 0)
            { childSize.x = layoutRectSize.x; }

            if (UILayoutManager::GetFlexibleSize(child).y > 0)
            { childSize.y = layoutRectSize.y; }

            crt->SetAnchors(Vector2(-1));

            crt->SetMarginLeft( GetPaddingLeft() );
            if (GetChildrenHorizontalStretch() == Stretch::None)
            {
                HorizontalAlignment hAlign = GetChildrenHorizontalAlignment();
                if (hAlign == HorizontalAlignment::Center)
                {
                    crt->AddMarginLeft( (layoutRectSize.x - childSize.x)/2 );
                }
                else if (hAlign == HorizontalAlignment::Right)
                {
                    crt->AddMarginLeft( (layoutRectSize.x - childSize.x) );
                }
                crt->SetMarginRight( -(crt->GetMarginLeft() + childSize.x) );
            }
            else
            {
                crt->SetMarginRight( -(crt->GetMarginLeft() + layoutRectSize.x) );
            }

            crt->SetMarginBot( GetPaddingBot() );
            if (GetChildrenVerticalStretch() == Stretch::None)
            {
                VerticalAlignment vAlign = GetChildrenVerticalAlignment();
                if (vAlign == VerticalAlignment::Center)
                {
                    crt->AddMarginBot( (layoutRectSize.y - childSize.y)/2 );
                }
                else if (vAlign == VerticalAlignment::Top)
                {
                    crt->AddMarginBot( (layoutRectSize.y - childSize.y) );
                }
                crt->SetMarginTop( -(crt->GetMarginBot() + childSize.y) );
            }
            else
            {
                crt->SetMarginTop( -(crt->GetMarginBot() + layoutRectSize.y));
            }
            break;
        }
    }
}

Vector2i UIFrameLayout::_GetMinSize() const
{
    Vector2i minSize = GetPaddingSize();
    for (GameObject *child : gameObject->GetChildren())
    {
        if (child->HasComponent<ILayoutElement>())
        {
            minSize += UILayoutManager::GetMinSize(child);
            break;
        }
    }
    return minSize;
}

Vector2i UIFrameLayout::_GetPreferredSize() const
{
    Vector2i prefSize = GetPaddingSize();
    for (GameObject *child : gameObject->GetChildren())
    {
        if (child->HasComponent<ILayoutElement>())
        {
            prefSize += UILayoutManager::GetPreferredSize(child);
            break;
        }
    }
    return prefSize;
}
