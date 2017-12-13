#include "Bang/UILabel.h"

#include "Bang/Font.h"
#include "Bang/Input.h"
#include "Bang/UIMask.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/SystemClipboard.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"

USING_NAMESPACE_BANG

UILabel::UILabel()
{
    SetSelectable(false);
}

UILabel::~UILabel()
{
}

void UILabel::OnStart()
{
    Component::OnStart();

    EventEmitter<IFocusListener>::RegisterListener(this);

    ResetSelection();
    SetSelectAllOnFocus(true);
    UpdateSelectionQuadRenderer();
}

void UILabel::OnUpdate()
{
    Component::OnUpdate();

    if (UICanvas::GetActive(this)->HasFocus(this))
    {
        if (IsSelectable())
        {
            if (!HasJustFocusChanged())
            {
                if (m_firstSelectAll &&
                    Input::GetMouseButtonDown(MouseButton::Left))
                {
                    m_firstSelectAll = false;
                }
                if (!m_firstSelectAll) { HandleMouseSelection(); }
            }

            HandleClipboardCopy();
        }
        else
        {
            ResetSelection();
        }
        UpdateSelectionQuadRenderer();
    }
}

void UILabel::SetCursorIndex(int index) { m_cursorIndex = index; }
void UILabel::SetSelectionIndex(int index) { m_selectionIndex = index; }

void UILabel::SetSelectable(bool selectable)
{
    m_selectable = selectable;
    SetFocusEnabled(selectable);
    if (p_selectionQuad) { p_selectionQuad->SetEnabled(selectable); }
}
void UILabel::SetSelection(int cursorIndex, int selectionIndex)
{
    SetCursorIndex(cursorIndex);
    SetSelectionIndex(selectionIndex);
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
    UpdateSelectionQuadRenderer();
}
void UILabel::SelectAll()
{
    SetSelection(GetText()->GetContent().Size(), 0);
}
void UILabel::SetSelectAllOnFocus(bool selectAllOnFocus)
{
    m_selectAllOnFocusTaken = selectAllOnFocus;
}


bool UILabel::IsSelectable() const
{
    return m_selectable;
}

int UILabel::GetSelectionBeginIndex() const
{
    return Math::Min(GetCursorIndex(), GetSelectionIndex());
}

int UILabel::GetSelectionEndIndex() const
{
    return Math::Max(GetCursorIndex(), GetSelectionIndex());
}

float UILabel::GetCursorXViewportNDC(int cursorIndex) const
{
    return GetTextParentRT()->FromLocalPointNDCToViewportPointNDC(
                            Vector2(GetCursorXLocalNDC(cursorIndex), 0) ).x;
}

float UILabel::GetCursorXLocalNDC(int cursorIndex) const
{
    float localTextX = 0.0f;
    const int textLength = GetText()->GetContent().Size();
    if (cursorIndex > 0 && cursorIndex < textLength) // Between two chars
    {
        Rect currentCharRect = GetText()->GetCharRectLocalNDC(cursorIndex - 1);
        Rect nextCharRect = GetText()->GetCharRectLocalNDC(cursorIndex);
        localTextX = (currentCharRect.GetMax().x + nextCharRect.GetMin().x) / 2.0f;
    }
    else if (!GetText()->GetCharRectsLocalNDC().IsEmpty()) // Begin or end
    {
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

bool UILabel::IsSelectAllOnFocus() const
{
    return m_selectAllOnFocusTaken;
}

int UILabel::GetClosestCharIndexTo(const Vector2 &coordsLocalNDC)
{
    int closestCharIndex = 0;
    float minDist = Math::Infinity<float>();
    const Array<Rect>& charRectsNDC = GetText()->GetCharRectsLocalNDC();
    for (int i = 0; i < charRectsNDC.Size(); ++i)
    {
        const Rect &cr = charRectsNDC[i];
        float distToMinX = Math::Abs(coordsLocalNDC.x - cr.GetMin().x);
        if (distToMinX < minDist)
        {
            minDist = distToMinX;
            closestCharIndex = i;
        }

        float distToMaxX = Math::Abs(coordsLocalNDC.x - cr.GetMax().x);
        if (distToMaxX < minDist)
        {
            minDist = distToMaxX;
            closestCharIndex = i + 1;
        }
    }
    return closestCharIndex;
}

int UILabel::GetCursorIndex() const { return m_cursorIndex; }
int UILabel::GetSelectionIndex() const { return m_selectionIndex; }
bool UILabel::IsSelectingWithMouse() const { return m_selectingWithMouse; }

UIMask *UILabel::GetMask() const { return p_mask; }
UITextRenderer *UILabel::GetText() const { return p_text; }

void UILabel::OnFocusTaken(IFocusable *focusable)
{
    IFocusListener::OnFocusTaken(focusable);
    ASSERT(focusable == this);

    if (IsSelectAllOnFocus() && IsSelectable())
    {
        m_firstSelectAll = true;
        SelectAll();
    }
    else { ResetSelection(); }

    UpdateSelectionQuadRenderer();
}

void UILabel::OnFocusLost(IFocusable *focusable)
{
    IFocusListener::OnFocusLost(focusable);
    ASSERT(focusable == this);

    ResetSelection();
    UpdateSelectionQuadRenderer();
    m_selectingWithMouse = false;
}


RectTransform *UILabel::GetTextParentRT() const
{
    return GetText()->GetGameObject()->GetParent()->GetRectTransform();
}
bool UILabel::IsShiftPressed() const
{
    return Input::GetKey(Key::LShift) || Input::GetKey(Key::RShift);
}

void UILabel::HandleClipboardCopy()
{
    if ( (Input::GetKey(Key::LControl) || Input::GetKey(Key::RControl)) )
    {
        String selectedText = GetSelectedText();
        if ( Input::GetKeyDown(Key::C) && selectedText.Size() > 0 )
        {
            SystemClipboard::Set( selectedText );
        }
    }
}

void UILabel::HandleMouseSelection()
{
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        m_selectingWithMouse = true;
    }
    else if (Input::GetMouseButtonUp(MouseButton::Left))
    {
        m_selectingWithMouse = false;
    }

    // Find the closest visible char bounds to the mouse position
    if (Input::GetMouseButton(MouseButton::Left))
    {
        Vector2 mouseCoordsLocalNDC = Input::GetMousePositionNDC();
        mouseCoordsLocalNDC = GetTextParentRT()->FromViewportPointNDCToLocalPointNDC(
                                            Vector2(mouseCoordsLocalNDC) );
        int closestCharIndex = GetClosestCharIndexTo(mouseCoordsLocalNDC);
        SetCursorIndex(closestCharIndex);

        // Move the selection index accordingly
        if (!IsShiftPressed() && Input::GetMouseButtonDown(MouseButton::Left))
        {
            ResetSelection();
        }
        if (!IsSelectingWithMouse()) { ResetSelection(); }
    }

    if (Input::GetMouseButtonDoubleClick(MouseButton::Left))
    {
        SelectAll();
    }
}

void UILabel::UpdateSelectionQuadRenderer()
{
    float cursorX     = GetCursorXViewportNDC( GetCursorIndex() );
    float selectionX  = GetCursorXViewportNDC( GetSelectionIndex() );

    float lineSkipPx  = GetText()->GetFont()->GetLineSkip(
                                                GetText()->GetTextSize() ) + 1;
    float lineSkipNDC = GL::FromViewportAmountToViewportAmountNDC( Vector2(0, lineSkipPx) ).y;

    Rect r = GetText()->GetContentViewportNDCRect();
    Vector2 p1(Math::Min(cursorX, selectionX), r.GetMax().y - lineSkipNDC);
    Vector2 p2(Math::Max(cursorX, selectionX), r.GetMax().y);

    RectTransform *textParentRT = GetTextParentRT();
    p1 = textParentRT->FromViewportPointNDCToLocalPointNDC(p1);
    p2 = textParentRT->FromViewportPointNDCToLocalPointNDC(p2);

    RectTransform *quadRT = p_selectionQuad->GetRectTransform();
    quadRT->SetAnchorMin( Vector2::Min(p1, p2) );
    quadRT->SetAnchorMax( Vector2::Max(p1, p2) );
}

UILabel *UILabel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UILabel *label = go->AddComponent<UILabel>();

    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::Full);
    vl->SetChildrenHorizontalStretch(Stretch::Full);
    vl->SetChildrenVerticalAlignment(VerticalAlignment::Center);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2(1.0f) );

    UIMask *mask = go->AddComponent<UIMask>();
    go->AddComponent<UIImageRenderer>(); // Quad mask
    label->p_mask = mask;

    GameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetTextSize(12);
    text->SetWrapping(false);

    UILayoutElement *textLE = textContainer->AddComponent<UILayoutElement>();
    textLE->SetFlexibleSize( Vector2(1.0f) );

    label->p_text = text;

    GameObject *selectionQuadGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *selectionQuad = selectionQuadGo->AddComponent<UIImageRenderer>();
    selectionQuad->SetTint(Color::LightBlue);
    label->p_selectionQuad = selectionQuadGo;

    go->SetAsChild(selectionQuadGo);
    go->SetAsChild(textContainer);

    label->ResetSelection();
    label->UpdateSelectionQuadRenderer();

    return label;
}

