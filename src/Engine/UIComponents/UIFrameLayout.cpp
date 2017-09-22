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

    Vector2i availablePx = layoutRect.GetSize();

    for (GameObject *child : gameObject->GetChildren())
    {
        ILayoutElement *layoutElm = child->GetComponent<ILayoutElement>();
        RectTransform *crt = child->GetComponent<RectTransform>();
        if (crt && layoutElm)
        {
            Vector2i childSize = layoutElm->GetTotalMinSize();
            childSize += GetPaddingSize();
            availablePx -= childSize;
            availablePx = Vector2i::Max(availablePx, Vector2i::Zero);

            Vector2i cFlexibleSize = layoutElm->GetTotalFlexiblePxSize();
            childSize += Vector2i::Min(cFlexibleSize, availablePx);

            crt->SetAnchors(Vector2(-1,-1), Vector2(1,1));
            crt->SetMarginLeft ( GetPaddingLeft() );
            crt->SetMarginBot  ( GetPaddingBot() );
            crt->SetMarginRight( GetPaddingRight() );
            crt->SetMarginTop  ( GetPaddingTop() );
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
