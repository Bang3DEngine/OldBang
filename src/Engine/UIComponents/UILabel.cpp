#include "Bang/UILabel.h"

#include "Bang/UIMask.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"

USING_NAMESPACE_BANG

UILabel::UILabel()
{
}

UILabel::~UILabel()
{
}

void UILabel::OnUpdate()
{
    Component::OnUpdate();

    // Render Selection Quad
    RectTransform *parentRT = GetParentRT();
    Rect limits = parentRT->GetScreenSpaceRectNDC();
    float cursorX    = GetCursorXGlobalNDC( GetCursorIndex() );
    float selectionX = GetCursorXGlobalNDC( GetSelectionIndex() );
    Vector2 p1(cursorX,    limits.GetMin().y);
    Vector2 p2(selectionX, limits.GetMax().y);
    p1 = parentRT->FromGlobalNDCToLocalNDC(p1);
    p2 = parentRT->FromGlobalNDCToLocalNDC(p2);

    RectTransform *selRT = p_selectionQuad->GetComponent<RectTransform>();
    selRT->SetAnchorMin( Vector2::Min(p1, p2) );
    selRT->SetAnchorMax( Vector2::Max(p1, p2) );
}

void UILabel::SetCursorIndex(int index) { m_cursorIndex = index; }
void UILabel::SetSelectionIndex(int index) { m_selectionIndex = index; }
void UILabel::SetSelection(int beginIndex, int endIndex)
{
    SetCursorIndex(beginIndex);
    SetSelectionIndex(endIndex);
}

String UILabel::GetSelectedText() const
{
    if (GetCursorIndex() == GetSelectionIndex()) { return ""; }
    if (GetSelectionBeginIndex() >= GetSelectionEndIndex()) { return ""; }
    return GetText()->GetContent().SubString(GetSelectionBeginIndex(),
                                             GetSelectionEndIndex()-1);
}

void UILabel::ResetSelection()
{
    SetSelectionIndex( GetCursorIndex() );
}

int UILabel::GetSelectionBeginIndex() const
{
    return Math::Min(GetCursorIndex(), GetSelectionIndex());
}

int UILabel::GetSelectionEndIndex() const
{
    return Math::Max(GetCursorIndex(), GetSelectionIndex());
}

float UILabel::GetCursorXGlobalNDC(int cursorIndex) const
{
    return GetParentRT()->FromLocalNDCToGlobalNDC(
                            Vector2(GetCursorXLocalNDC(cursorIndex), 0) ).x;
}

float UILabel::GetCursorXLocalNDC(int cursorIndex) const
{
    float localTextX = 0.0f;
    const int textLength = GetText()->GetContent().Size();
    if (cursorIndex > 0 && cursorIndex < textLength)
    {
        // In case we are between two characters
        Rect currentCharRect = GetText()->GetCharRectLocalNDC(cursorIndex - 1);
        Rect nextCharRect = GetText()->GetCharRectLocalNDC(cursorIndex);
        localTextX = (currentCharRect.GetMax().x + nextCharRect.GetMin().x) / 2.0f;
    }
    else if (!GetText()->GetCharRectsLocalNDC().IsEmpty())
    {
        // In case we are at the beginning or at the end of the text
        localTextX = (cursorIndex == 0 ?
                      GetText()->GetCharRectsLocalNDC().Front().GetMin().x :
                      GetText()->GetCharRectsLocalNDC().Back() .GetMax().x);
    }
    else
    {
        HorizontalAlignment hAlign = GetText()->GetHorizontalAlignment();
        if (hAlign == HorizontalAlignment::Left)   { return -1; }
        if (hAlign == HorizontalAlignment::Center) { return  0; }
        if (hAlign == HorizontalAlignment::Right)  { return  1; }
    }

    return Vector2(localTextX, 0).x;
}

int UILabel::GetCursorIndex() const { return m_cursorIndex; }
int UILabel::GetSelectionIndex() const { return m_selectionIndex; }

UIMask *UILabel::GetMask() const { return p_mask; }
UITextRenderer *UILabel::GetText() const { return p_text; }

UILabel *UILabel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UILabel *label = go->AddComponent<UILabel>();

    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();

    UIMask *mask = go->AddComponent<UIMask>();
    go->AddComponent<UIImageRenderer>(); // Quad mask
    label->p_mask = mask;

    GameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetTextSize(12);
    text->SetWrapping(false);
    label->p_text = text;

    GameObject *selectionQuadGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *selectionQuad = selectionQuadGo->AddComponent<UIImageRenderer>();
    selectionQuad->SetTint(Color::LightBlue);
    label->p_selectionQuad = selectionQuadGo;

    go->AddChild(selectionQuadGo);
    go->AddChild(textContainer);

    return label;
}

RectTransform *UILabel::GetParentRT() const
{
    return GetGameObject()->GetParent()->GetComponent<RectTransform>();
}


