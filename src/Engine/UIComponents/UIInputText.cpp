#include "Bang/UIInputText.h"

#include "Bang/Font.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/UIMask.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/SystemClipboard.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

const Vector2i UIInputText::LookAheadOffsetPx = Vector2i(5);

UIInputText::UIInputText()
{
}

UIInputText::~UIInputText()
{
}

void UIInputText::OnUpdate()
{
    Component::OnUpdate();

    UIFocusTaker *ft = gameObject->GetComponent<UIFocusTaker>();
    if ( ft->HasFocus() )
    {
        const bool wasSelecting = (GetSelectionIndex() != GetCursorIndex());

        HandleTyping();
        HandleCursorIndices(wasSelecting);
        UpdateCursorRenderer();
    }
    p_cursor->SetEnabled( ft->HasFocus() );
}

void UIInputText::UpdateCursorRenderer()
{
    // Cursor "I" position update
    float cursorX  = GetLabel()->GetCursorXLocalNDC( GetCursorIndex() );
    float lineSkip = GetText()->GetFont()->GetLineSkip();
    float lineSkipNDC = GetLabelRT()->FromPixelsAmountToLocalNDC(
                                              Vector2i(0, lineSkip)).y;
    Vector2 minPoint(cursorX, 1 - lineSkipNDC);
    Vector2 maxPoint(cursorX, 1);
    const Vector2 cursorSize = GetLabelRT()->FromPixelsAmountToLocalNDC(Vector2i(3,0));
    p_cursor->gameObject->GetComponent<RectTransform>()
            ->SetAnchors(minPoint - cursorSize, maxPoint + cursorSize);
}

void UIInputText::UpdateTextScrolling()
{
    Vector2i prevScrollPx = p_scrollArea->GetScrolling();
    p_scrollArea->SetScrolling( Vector2i::Zero ); // To make things easier
    GetText()->RegenerateCharQuadsVAO();

    Vector2 scrollNDC = Vector2::Zero;
    Rect labelLimits = GetLabelRT()->GetScreenSpaceRectNDC();
    Rect contentRectNDC = GetText()->GetContentGlobalNDCRect();
    if (contentRectNDC.GetWidth() < labelLimits.GetWidth() ||
        GetCursorIndex() == 0)
    {
        p_scrollArea->SetScrolling( Vector2i::Zero );
    }
    else
    {
        p_scrollArea->SetScrolling(prevScrollPx);
        GetText()->RegenerateCharQuadsVAO();
        contentRectNDC = GetText()->GetContentGlobalNDCRect();
        float cursorX = GetLabel()->GetCursorXGlobalNDC( GetCursorIndex() );
        float lookAheadNDC = GL::FromPixelsAmountToGlobalNDC(LookAheadOffsetPx).x;
        if (cursorX < labelLimits.GetMin().x)
        {
            scrollNDC.x = labelLimits.GetMin().x - cursorX + lookAheadNDC;
        }
        else if (cursorX > labelLimits.GetMax().x)
        {
            scrollNDC.x = labelLimits.GetMax().x - cursorX - lookAheadNDC;
        }
        else
        {
            if (contentRectNDC.GetMin().x < labelLimits.GetMin().x &&
                contentRectNDC.GetMax().x < labelLimits.GetMax().x)
            {
                scrollNDC.x = labelLimits.GetMax().x -
                              contentRectNDC.GetMax().x -
                              lookAheadNDC;
            }
        }

        Vector2i scrollPx = GL::FromGlobalNDCToPixelsAmount(scrollNDC);
        p_scrollArea->SetScrolling(prevScrollPx + scrollPx);
    }
}

void UIInputText::HandleTyping()
{
    String inputText = Input::PollInputText();
    bool resetSelection = false;

    // First we handle text deletion
    if (!GetText()->GetContent().IsEmpty())
    {
        int offsetCursor = 0;
        int offsetSelection = 1;
        bool removeText = false;
        bool selecting = GetSelectedText().Size() > 0;
        if (Input::GetKeyDownRepeat(Key::Delete))
        {
            if (selecting) { offsetSelection += -1; }
            removeText = true;
        }
        else if (Input::GetKeyDownRepeat(Key::BackSpace))
        {
            offsetCursor    += (selecting ? 0 : -1);
            offsetSelection += -1;
            removeText = true;
        }

        if (removeText)
        {
            if (GetCursorIndex() > GetSelectionIndex())
            {   // Swap indices
                int oldSelectionIndex = GetSelectionIndex();
                GetLabel()->SetSelectionIndex( GetCursorIndex() );
                SetCursorIndex(oldSelectionIndex);
            }

            SetCursorIndex( GetCursorIndex() + offsetCursor);
            GetLabel()->SetSelectionIndex(GetSelectionIndex() + offsetSelection);
            ReplaceSelectedText("");

            resetSelection = true;
        }
    }

    // Key typing handling
    if (!inputText.IsEmpty())
    {
        String content = GetText()->GetContent();
        content.Insert(GetCursorIndex(), inputText);
        GetText()->SetContent(content);
        SetCursorIndex( GetCursorIndex() + inputText.Size() );
        resetSelection = true;
    }

    if ( (Input::GetKey(Key::LControl) || Input::GetKey(Key::RControl)) )
    {
        String selectedText = GetSelectedText();
        if ( Input::GetKeyDown(Key::C) && selectedText.Size() > 0 )
        {
            SystemClipboard::Set(selectedText);
        }
        else if ( Input::GetKeyDown(Key::X) && selectedText.Size() > 0 )
        {
            SystemClipboard::Set(selectedText);
            ReplaceSelectedText("");
        }
        else if ( Input::GetKeyDownRepeat(Key::V) )
        {
            String clipboardText = SystemClipboard::Get();
            ReplaceSelectedText(clipboardText);
            SetCursorIndex( GetCursorIndex() + clipboardText.Size());
            resetSelection = true;
        }
    }

    if (resetSelection) { ResetSelection(); }
    if (Input::GetKeyDown(Key::End))
    {
        SetCursorIndex( GetText()->GetContent().Size() );
    }
    else if (Input::GetKeyDown(Key::Home)) { SetCursorIndex(0); }
}

void UIInputText::HandleCursorIndices(bool wasSelecting)
{
    // Here we will move the selection indices either by mouse or arrows...
    UIFocusTaker *ft = gameObject->GetComponent<UIFocusTaker>();
    if (ft->IsMouseOver() && Input::GetMouseButtonDown(MouseButton::Left))
    {
        if (!IsShiftPressed()) { ResetSelection(); }
        m_selectingWithMouse = true;
    }
    else if (Input::GetMouseButtonUp(MouseButton::Left))
    {
        m_selectingWithMouse = false;
    }

    HandleMouseSelection();
    HandleKeySelection(wasSelecting);
}

int UIInputText::GetCursorIndex() const { return GetLabel()->GetCursorIndex(); }
int UIInputText::GetSelectionIndex() const { return GetLabel()->GetSelectionIndex(); }
UILabel *UIInputText::GetLabel() const { return p_label; }

RectTransform *UIInputText::GetLabelRT() const
{
    return GetLabel()->GetGameObject()->GetComponent<RectTransform>();
}

RectTransform *UIInputText::GetTextRT() const
{
    return GetText()->gameObject->GetComponent<RectTransform>();
}

RectTransform *UIInputText::GetRT() const
{
    return gameObject->GetComponent<RectTransform>();
}

void UIInputText::HandleKeySelection(bool wasSelecting)
{
    // Get cursor advance 1/-1
    int cursorIndexAdvance = 0;
    if (Input::GetKeyDownRepeat(Key::Right)) { cursorIndexAdvance = 1; }
    if (Input::GetKeyDownRepeat(Key::Left)) { cursorIndexAdvance = -1; }

    if (cursorIndexAdvance != 0 &&
        (Input::GetKey(Key::LControl) ||
         Input::GetKey(Key::RControl))
       )
    {
        bool fwd = (cursorIndexAdvance > 0);
        int startIdx = GetCursorIndex() + (fwd ? 0 : -1);
        cursorIndexAdvance = GetWordSplitIndex(startIdx, fwd) - GetCursorIndex();
        cursorIndexAdvance += (fwd ? 0 : 1);
    }

    // Advance the the cursor index, and clamp it
    SetCursorIndex(Math::Clamp(GetCursorIndex() + cursorIndexAdvance,
                               0, GetText()->GetContent().Size()) );

    // Selection resetting handling
    bool doingSelection = IsShiftPressed() || m_selectingWithMouse;
    if (!doingSelection)
    {
        if (wasSelecting && cursorIndexAdvance != 0)
        {
            bool moveCursorToSelectionBoundary =
              (cursorIndexAdvance == -1 && GetCursorIndex() > GetSelectionIndex()) ||
              (cursorIndexAdvance ==  1 && GetCursorIndex() < GetSelectionIndex());

            if (moveCursorToSelectionBoundary)
            {
                SetCursorIndex( GetSelectionIndex() );
            }
            else { SetCursorIndex( GetCursorIndex() - cursorIndexAdvance); }

            if (!IsShiftPressed()) { ResetSelection(); }
        }
    }
    if (!doingSelection && !wasSelecting) { ResetSelection(); }
}

void UIInputText::HandleMouseSelection()
{
    // Find the closest visible char bounds to the mouse position
    if (Input::GetMouseButton(MouseButton::Left))
    {
        float minDist = Math::Infinity<float>();
        int closestCharRectIndex = 0;
        float mouseCoordsX_NDC = Input::GetMouseCoordsNDC().x;
        mouseCoordsX_NDC = GetTextRT()->FromGlobalNDCToLocalNDC(
                                          Vector2(mouseCoordsX_NDC) ).x;

        const Array<Rect>& charRectsNDC = GetText()->GetCharRectsLocalNDC();
        for (int i = 0; i < charRectsNDC.Size(); ++i)
        {
            const Rect &cr = charRectsNDC[i];
            float distToMinX = Math::Abs(mouseCoordsX_NDC - cr.GetMin().x);
            if (distToMinX < minDist)
            {
                minDist = distToMinX;
                closestCharRectIndex = i;
            }

            float distToMaxX = Math::Abs(mouseCoordsX_NDC - cr.GetMax().x);
            if (distToMaxX < minDist)
            {
                minDist = distToMaxX;
                closestCharRectIndex = i + 1;
            }
        }
        SetCursorIndex(closestCharRectIndex);

        // Move the selection index accordingly
        if (!IsShiftPressed() && Input::GetMouseButtonDown(MouseButton::Left))
        {
           ResetSelection();
        }
        if (!m_selectingWithMouse) { ResetSelection(); }
    }

    if (Input::GetMouseButtonDoubleClick(MouseButton::Left))
    {
        // SelectAll();
    }
}

void UIInputText::SetCursorIndex(int index)
{
    GetLabel()->SetCursorIndex(index);
    UpdateCursorRenderer();
}

void UIInputText::SetSelection(int selectionBeginIndex,
                               int selectionEndIndex)
{
    GetLabel()->SetSelection(selectionBeginIndex, selectionEndIndex);
    UpdateCursorRenderer();
}

String UIInputText::GetSelectedText() const { return GetLabel()->GetSelectedText(); }

void UIInputText::ReplaceSelectedText(const String &replaceStr)
{
    String content = GetText()->GetContent();
    int minIndex = GetLabel()->GetSelectionBeginIndex();
    int maxIndex = GetLabel()->GetSelectionEndIndex();

    if (minIndex >= 0 && minIndex <= content.Size() &&
        maxIndex >= 0 && maxIndex <= content.Size())
    {
        content.Remove(minIndex, maxIndex);
        content.Insert(minIndex, replaceStr);
        GetText()->SetContent(content);
    }

    SetCursorIndex(minIndex);
    ResetSelection();
}

void UIInputText::ResetSelection() { GetLabel()->ResetSelection(); }
void UIInputText::SelectAll()
{
    SetSelection(0, GetText()->GetContent().Size());
}

UITextCursor *UIInputText::GetCursor() const
{
    return p_cursor;
}

UITextRenderer *UIInputText::GetText() const
{
    return p_label->GetText();
}

UIImageRenderer *UIInputText::GetBackground() const
{
    return p_background;
}

bool UIInputText::IsShiftPressed() const
{
    return Input::GetKey(Key::LShift) || Input::GetKey(Key::RShift);
}

UIInputText *UIInputText::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);
    REQUIRE_COMPONENT(go, UIFocusTaker);

    UIInputText *inputText = go->AddComponent<UIInputText>();

    UIFocusTaker *ft = go->GetComponent<UIFocusTaker>();
    ft->SetDefaultFocusAction(FocusAction::TakeIt);

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);
    inputText->p_background = bg;

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    scrollArea->GetMask()->SetMasking(true);
    inputText->p_scrollArea = scrollArea;

    GameObject *cursorGo = GameObjectFactory::CreateUIGameObject();
    UITextCursor *cursor = cursorGo->AddComponent<UITextCursor>();
    inputText->p_cursor = cursor;

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetMask()->SetMasking(false);
    label->GetGameObject()->GetComponent<RectTransform>()->SetMargins(5, 2, 5, 2);
    inputText->p_label = label;

    inputText->SetCursorIndex( inputText->GetText()->GetContent().Size() );
    inputText->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    inputText->GetText()->SetVerticalAlign(VerticalAlignment::Center);
    inputText->GetText()->SetWrapping(false);
    inputText->ResetSelection();
    inputText->UpdateCursorRenderer();

    scrollArea->GetContainer()->AddChild(label->gameObject);
    label->GetGameObject()->AddChild(cursorGo);

    return inputText;
}

bool UIInputText::IsDelimiter(char initialChar, char curr) const
{
    bool initialIsLetter = String::IsLetter(initialChar);
    bool differentCase   = initialIsLetter &&
                           (String::IsUpperCase(initialChar) !=
                            String::IsUpperCase(curr));
    return differentCase ||
           (!String::IsLetter(curr) && !String::IsNumber(curr));
}

int UIInputText::GetWordSplitIndex(int startIndex, bool forward) const
{
    const String &content = GetText()->GetContent();

    if (startIndex <= 0 && !forward) { return startIndex; }
    if (startIndex >= content.Size()-1 && forward) { return startIndex; }

    int i = startIndex;
    while ( i >= 0 && i < content.Size() &&
            IsDelimiter(content[startIndex], content[i]) )
    {
        i += (forward ? 1 : -1);
    }

    for (;
         (forward ? (i < content.Size()) : (i >= 0));
         i += (forward ? 1 : -1))
    {
        if ( IsDelimiter(content[startIndex], content[i]) )
        {
            return i;
        }
    }

    return i;
}

void UIInputText::OnFocusTaken()
{
    IFocusListener::OnFocusTaken();
    Input::StartTextInput();
}

void UIInputText::OnFocusLost()
{
    IFocusListener::OnFocusLost();
    ResetSelection();
    Input::StopTextInput();
    m_selectingWithMouse = false;
    UpdateCursorRenderer();
}
