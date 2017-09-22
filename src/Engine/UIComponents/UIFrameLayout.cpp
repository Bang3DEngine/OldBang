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
    RectTransform *layoutRT = GetGameObject()->GetComponent<RectTransform>();
    ENSURE(layoutRT);
    Recti layoutRect = layoutRT->GetScreenSpaceRectPx();
    layoutRect = Recti(layoutRect.GetMin() + GetPaddingLeftBot(),
                       layoutRect.GetMax() - GetPaddingRightTop());

    Vector2i availablePx = layoutRect.GetSize();

    for (GameObject *child : gameObject->GetChildren())
    {
        RectTransform *crt = child->GetComponent<RectTransform>();
        ILayoutElement *layoutElm = child->GetComponent<ILayoutElement>();
        if (crt && layoutElm)
        {
            Vector2i childSize = layoutElm->GetTotalMinSize();
            availablePx -= childSize;
            availablePx = Vector2i::Max(availablePx, Vector2i::Zero);

            Vector2i cFlexibleSize = layoutElm->GetTotalFlexiblePxSize();
            childSize += Vector2i::Min(cFlexibleSize, availablePx);

            crt->SetAnchors(Vector2(-1));

            crt->SetMarginLeft( GetPaddingLeft() );
            if (GetChildrenHorizontalStretch() == Stretch::None)
            {
                HorizontalAlignment hAlign = GetChildrenHorizontalAlignment();
                if (hAlign == HorizontalAlignment::Center)
                {
                    crt->SetMarginLeft( crt->GetMarginLeft() +
                                        (layoutRect.GetWidth() - childSize.x)/2 );
                }
                else if (hAlign == HorizontalAlignment::Right)
                {
                    crt->SetMarginLeft( crt->GetMarginLeft() +
                                        (layoutRect.GetWidth() - childSize.x) );
                }
                crt->SetMarginRight( -(crt->GetMarginLeft() + childSize.x) );
            }
            else
            {
                crt->SetMarginRight( -(crt->GetMarginLeft() + layoutRect.GetWidth()) );
            }

            crt->SetMarginBot( GetPaddingBot() );
            if (GetChildrenVerticalStretch() == Stretch::None)
            {
                VerticalAlignment vAlign = GetChildrenVerticalAlignment();
                if (vAlign == VerticalAlignment::Center)
                {
                    crt->SetMarginBot( crt->GetMarginBot() +
                                        (layoutRect.GetHeight() - childSize.y)/2 );
                }
                else if (vAlign == VerticalAlignment::Top)
                {
                    crt->SetMarginBot( crt->GetMarginBot() +
                                        (layoutRect.GetHeight() - childSize.y) );
                }
                crt->SetMarginTop( -(crt->GetMarginBot() + childSize.y) );
            }
            else
            {
                crt->SetMarginTop( -(crt->GetMarginBot() + layoutRect.GetHeight()));
            }
            break;
        }
    }
}

Vector2i UIFrameLayout::CalculateTotalMinSize() const
{
    Vector2i minSize = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        minSize += layoutElm->GetTotalMinSize();
    }
    return minSize;
}

Vector2i UIFrameLayout::CalculateTotalPreferredSize() const
{
    Vector2i prefSize = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        prefSize += layoutElm->GetTotalPreferredSize();
    }
    return prefSize;
}

Vector2i UIFrameLayout::CalculateTotalFlexiblePxSize() const
{
    Vector2i flexSize = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        flexSize += layoutElm->GetTotalFlexiblePxSize();
    }
    return flexSize;
}
