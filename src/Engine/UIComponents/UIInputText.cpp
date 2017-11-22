#include "Bang/UIInputText.h"

#include "Bang/Font.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/UIMask.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/SystemClipboard.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutManager.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

const Vector2i UIInputText::LookAheadOffsetPx = Vector2i(5);
const int UIInputText::MarginX = 5;
const int UIInputText::MarginY = 2;

UIInputText::UIInputText()
{
}

UIInputText::~UIInputText()
{
}

void UIInputText::OnUpdate()
{
    Component::OnUpdate();

    UIFocusable *focusable = GetGameObject()->GetComponent<UIFocusable>();
    bool hasFocus = focusable->HasFocus();
    if (hasFocus)
    {
        const bool wasSelecting = (GetSelectionIndex() != GetCursorIndex());

        HandleTyping();
        HandleCursorIndices(wasSelecting);
        UpdateTextScrolling();
        UpdateCursorRenderer();
    }
    p_cursor->SetEnabled(hasFocus);
}

void UIInputText::UpdateCursorRenderer()
{
    // Cursor "I" position update
    float cursorX  = GetLabel()->GetCursorXGlobalNDC( GetCursorIndex() );
    float lineSkip = GetText()->GetFont()->GetLineSkip() + 1;
    float lineSkipNDC = GL::FromPixelsAmountToGlobalNDC(Vector2(0, lineSkip)).y;

    Vector2 minPoint, maxPoint;
    if (!GetText()->GetContent().IsEmpty())
    {
        Rect textRect = GetText()->GetContentGlobalNDCRect();
        minPoint = Vector2(cursorX, textRect.GetMax().y - lineSkipNDC);
        maxPoint = Vector2(cursorX, textRect.GetMax().y);
    }
    else
    {
        Rect limitsRect = GetLabelRT()->GetScreenSpaceRectNDC();
        minPoint = Vector2(cursorX, limitsRect.GetMin().y);
        maxPoint = Vector2(cursorX, limitsRect.GetMax().y);
    }

    const RectTransform *cParentRT = p_cursor->GetGameObject()->GetParent()->
                                                 GetComponent<RectTransform>();
    const Vector2 cursorSizeLocalNDC = cParentRT->FromPixelsAmountToLocalNDC(Vector2i(3,0));
    const Vector2 minPointLocalNDC = cParentRT->FromGlobalNDCToLocalNDC(minPoint);
    const Vector2 maxPointLocalNDC = cParentRT->FromGlobalNDCToLocalNDC(maxPoint);

    RectTransform *cRT = p_cursor->GetGameObject()->GetComponent<RectTransform>();
    cRT->SetAnchors(minPointLocalNDC - cursorSizeLocalNDC,
                    maxPointLocalNDC + cursorSizeLocalNDC);
}

void UIInputText::UpdateTextScrolling()
{
    Vector2i prevScrollPx = p_scrollArea->GetScrolling();
    p_scrollArea->SetScrolling( Vector2i::Zero );
    GetText()->RegenerateCharQuadsVAO();

    Vector2 scrollNDC = Vector2::Zero;
    Rect labelLimits = GetLabelRT()->GetScreenSpaceRectNDC();
    Rect contentRectNDC = GetText()->GetContentGlobalNDCRect();
    if (contentRectNDC.GetWidth() > labelLimits.GetWidth() && GetCursorIndex() > 0)
    {
        p_scrollArea->SetScrolling(prevScrollPx);
        GetText()->RegenerateCharQuadsVAO();
        contentRectNDC = GetText()->GetContentGlobalNDCRect();
        float cursorX = GetLabel()->GetCursorXGlobalNDC( GetCursorIndex() );
        float lookAheadNDC = GL::FromPixelsAmountToGlobalNDC(
                                                Vector2(LookAheadOffsetPx) ).x;
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

        Vector2i scrollPx( GL::FromGlobalNDCToPixelsAmount(scrollNDC) );
        p_scrollArea->SetScrolling(prevScrollPx + scrollPx);
    }
}

void UIInputText::HandleTyping()
{
    String inputText = Input::PollInputText();
    inputText = FilterAllowedInputText(inputText);

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
        // String content = GetText()->GetContent();
        // content.Insert(GetCursorIndex(), inputText);
        // GetText()->SetContent(content);
        ReplaceSelectedText(inputText);
        SetCursorIndex( GetCursorIndex() + inputText.Size() );
        resetSelection = true;
    }

    if ( (Input::GetKey(Key::LControl) || Input::GetKey(Key::RControl)) )
    {
        String selectedText = GetSelectedText();
        if ( Input::GetKeyDown(Key::X) && selectedText.Size() > 0 )
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

    if (Input::GetKeyDown(Key::End))
    {
        SetCursorIndex( GetText()->GetContent().Size() );
    }
    else if (Input::GetKeyDown(Key::Home)) { SetCursorIndex(0); }
    if (resetSelection) { ResetSelection(); }
}

void UIInputText::HandleCursorIndices(bool wasSelecting)
{
    // Here we will move the selection indices either by arrows...
    HandleKeySelection(wasSelecting);
}

String UIInputText::FilterAllowedInputText(const String &inputText)
{
    if (m_allowedCharacters.IsEmpty()) { return inputText; }

    String allowedText = "";
    for (char c : inputText)
    {
        if (m_allowedCharacters.Contains( String(c) )) { allowedText += c; }
    }
    return allowedText;
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
    return GetText()->GetGameObject()->GetComponent<RectTransform>();
}

RectTransform *UIInputText::GetRT() const
{
    return GetGameObject()->GetComponent<RectTransform>();
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
    bool doingSelection = IsShiftPressed() || GetLabel()->IsSelectingWithMouse();
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

    PROPAGATE(IValueChangedListener, OnValueChanged, SCAST<Object*>(this));
}

void UIInputText::ResetSelection() { GetLabel()->ResetSelection(); }

void UIInputText::SetAllowedCharacters(const String &allowedCharacters)
{
    m_allowedCharacters = allowedCharacters;
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

    UIInputText *inputText = go->AddComponent<UIInputText>();
    go->AddComponent<UIFocusable>();

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);
    inputText->p_background = bg;

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    scrollArea->GetMask()->SetMasking(true);
    inputText->p_scrollArea = scrollArea;

    GameObject *cursorGo = GameObjectFactory::CreateUIGameObject();
    UITextCursor *cursor = cursorGo->AddComponent<UITextCursor>();
    inputText->p_cursor = cursor;
    cursor->SetEnabled(false);

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->SetSelectable(true);
    label->GetMask()->SetMasking(false);
    label->GetGameObject()->GetComponent<RectTransform>()->
                                SetMargins(MarginX, MarginY, MarginX, MarginY);
    inputText->p_label = label;

    scrollArea->GetContainer()->SetAsChild(label->GetGameObject());
    label->GetGameObject()->SetAsChild(cursorGo);

    inputText->SetCursorIndex( inputText->GetText()->GetContent().Size() );
    inputText->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    inputText->GetText()->SetVerticalAlign(VerticalAlignment::Center);
    inputText->GetText()->SetWrapping(false);
    inputText->ResetSelection();
    inputText->UpdateCursorRenderer();

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
    Input::PollInputText();

    GetGameObject()->GetComponentInChildren<UILabel>()->OnFocusTaken();
}

void UIInputText::OnFocusLost()
{
    IFocusListener::OnFocusLost();
    UpdateCursorRenderer();

    GetGameObject()->GetComponentInChildren<UILabel>()->OnFocusLost();
}

void UIInputText::CalculateLayout(Axis axis)
{
    GameObject *textGo = GetText()->GetGameObject();

    Vector2i minSize = UILayoutManager::GetMinSize(textGo);
    Vector2i prefSize = UILayoutManager::GetPreferredSize(textGo);
    Vector2 flexSize = Vector2(1, 1);
    minSize  += Vector2i(MarginX, MarginY) * 2;
    prefSize += Vector2i(MarginX, MarginY) * 2;
    minSize.x = prefSize.x = -1;

    SetCalculatedLayout(axis, minSize.GetAxis(axis), prefSize.GetAxis(axis),
                        flexSize.GetAxis(axis));
}
