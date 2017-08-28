#include "Bang/UIInputText.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UIGameObject.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

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

    RetrieveReferences();

    UIGameObject *uiGo = SCAST<UIGameObject*>(GetGameObject());
    if ( uiGo->HasFocus() )
    {
        const bool wasSelecting = (m_selectionIndex != m_cursorIndex);
        HandleTyping();
        HandleCursorIndices(wasSelecting);
        UpdateCursorRenderersAndScrolling();
    }
    p_cursor->SetEnabled( uiGo->HasFocus() );
}

void UIInputText::UpdateCursorRenderersAndScrolling()
{
    UIGameObject *uiGo = SCAST<UIGameObject*>(GetGameObject());
    Rect limits = uiGo->rectTransform->GetScreenSpaceRectNDC();

    Vector2i prevScrolling = p_boxScrollArea->GetScrolling();
    p_boxScrollArea->SetScrolling( Vector2i::Zero ); // To make things easier

    // Cursor "I" position update and Selection quad rendering
    {
        {
            float cursorX = GetCursorXLocalNDC(m_cursorIndex);
            Vector2 minPoint(cursorX, -1);
            Vector2 maxPoint(cursorX,  1);
            const Vector2 cursorSize =
              p_label->rectTransform->FromPixelsAmountToLocalNDC(Vector2i(3,0));
            p_cursor->rectTransform->SetAnchors(minPoint - cursorSize,
                                                maxPoint + cursorSize);
        }

        // Selection quad
        {
            float cursorX    = GetCursorXGlobalNDC(m_cursorIndex);
            float selectionX = GetCursorXGlobalNDC(m_selectionIndex);
            Vector2 p1(cursorX,    limits.GetMin().y);
            Vector2 p2(selectionX, limits.GetMax().y);
            p1 = p_label->rectTransform->FromGlobalNDCToLocalNDC(p1);
            p2 = p_label->rectTransform->FromGlobalNDCToLocalNDC(p2);

            p_selectionQuad->rectTransform->SetAnchorMin( Vector2::Min(p1, p2) );
            p_selectionQuad->rectTransform->SetAnchorMax( Vector2::Max(p1, p2) );
        }
    }

    // Text Scrolling
    {
        Vector2 scrollNDC = Vector2::Zero;
        Rect labelLimits = p_label->rectTransform->GetScreenSpaceRectNDC();
        Rect contentRectNDC = GetText()->GetContentGlobalNDCRect();
        if (contentRectNDC.GetWidth() < labelLimits.GetWidth() ||
            m_cursorIndex == 0)
        {
            p_boxScrollArea->SetScrolling( Vector2i::Zero );
        }
        else
        {
            p_boxScrollArea->SetScrolling(prevScrolling);
            float cursorX = GetCursorXGlobalNDC(m_cursorIndex);
            float lookAheadNDC =
                RectTransform::FromPixelsAmountToGlobalNDC(LookAheadOffsetPx).x;
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
                                  contentRectNDC.GetMax().x;
                }
            }
            Vector2i scrollPx = RectTransform::FromGlobalNDCToPixelsAmount(scrollNDC);
            p_boxScrollArea->SetScrolling(prevScrolling + scrollPx);
        }
    }
}

void UIInputText::HandleTyping()
{
    String content = GetText()->GetContent();
    String inputText = Input::PollInputText();

    // First we handle text deletion
    if (!GetText()->GetContent().IsEmpty())
    {
        bool selecting = (m_cursorIndex != m_selectionIndex);
        int minIndex = Math::Min(m_cursorIndex, m_selectionIndex) +
                       (selecting ? 1 : 0);
        int maxIndex = Math::Max(m_cursorIndex, m_selectionIndex);

        int offset = 1;
        if (Input::GetKeyDownRepeat(Input::Key::Delete))
        {
            offset = (selecting ? -1 : 0);
        }
        else if (Input::GetKeyDownRepeat(Input::Key::BackSpace))
        {
            offset = -1;
        }
        else if (selecting && !inputText.IsEmpty()) { offset = -1; }

        minIndex += offset;
        maxIndex += offset;
        if (offset != 1 && minIndex >= 0 && maxIndex <= content.Size()-1)
        {
            content.Remove(minIndex, maxIndex);
            m_cursorIndex = minIndex;
            ResetSelection();
        }
    }

    // Key typing handling
    if (!inputText.IsEmpty())
    {
        content.Insert(m_cursorIndex, inputText);
        m_cursorIndex += inputText.Size();
        ResetSelection();
    }

    if (Input::GetKeyDown(Input::Key::End))
    {
        m_cursorIndex = GetText()->GetContent().Size();
    }
    else if (Input::GetKeyDown(Input::Key::Home)) { m_cursorIndex = 0; }

    if (content != GetText()->GetContent())
    {
        GetText()->SetContent(content);
        GetText()->RefreshMesh();
    }
}

void UIInputText::HandleCursorIndices(bool wasSelecting)
{
    // Here we will move the selection indices either by mouse or arrows...
    UIGameObject *uiGo = SCAST<UIGameObject*>(GetGameObject());
    if (uiGo->IsMouseOver() &&
        Input::GetMouseButtonDown(Input::MouseButton::Left))
    {
        ResetSelection();
        m_selectingWithMouse = true;
    }
    else if (Input::GetMouseButtonUp(Input::MouseButton::Left))
    {
        m_selectingWithMouse = false;
    }

    HandleMouseSelection();
    HandleKeySelection(wasSelecting);
}

float UIInputText::GetCursorXGlobalNDC(int cursorIndex) const
{
    return p_label->rectTransform->FromLocalNDCToGlobalNDC(
                            Vector2(GetCursorXLocalNDC(cursorIndex), 0) ).x;
}

void UIInputText::HandleKeySelection(bool wasSelecting)
{
    // Get cursor advance 1/-1
    int cursorIndexAdvance = 0;
    if (Input::GetKeyDownRepeat(Input::Key::Right)) { cursorIndexAdvance = 1; }
    if (Input::GetKeyDownRepeat(Input::Key::Left)) { cursorIndexAdvance = -1; }

    if (cursorIndexAdvance != 0 &&
        (Input::GetKey(Input::Key::LControl) ||
         Input::GetKey(Input::Key::RControl))
       )
    {
        bool fwd = (cursorIndexAdvance > 0);
        int startIdx = m_cursorIndex + (fwd ? 0 : -1);
        cursorIndexAdvance = GetWordSplitIndex(startIdx, fwd) - m_cursorIndex;
        cursorIndexAdvance += (fwd ? 0 : 1);
    }

    // Advance the the cursor index, and clamp it
    m_cursorIndex = Math::Clamp(m_cursorIndex + cursorIndexAdvance,
                                0, GetText()->GetContent().Size());

    // Selection resetting handling
    bool doingSelection = IsShiftPressed() || m_selectingWithMouse;
    if (!doingSelection)
    {
        if (wasSelecting && cursorIndexAdvance != 0)
        {
            bool moveCursorToSelectionBoundary =
              (cursorIndexAdvance == -1 && m_cursorIndex > m_selectionIndex) ||
              (cursorIndexAdvance ==  1 && m_cursorIndex < m_selectionIndex);

            if (moveCursorToSelectionBoundary)
            {
                m_cursorIndex = m_selectionIndex;
            }
            else { m_cursorIndex -= cursorIndexAdvance; }

            ResetSelection();
        }
    }
    if (!doingSelection && !wasSelecting) { ResetSelection(); }
}

void UIInputText::HandleMouseSelection()
{
    // Find the closest visible char bounds to the mouse position
    if (Input::GetMouseButton(Input::MouseButton::Left))
    {
        float minDist = Math::Infinity<float>();
        int closestCharRectIndex = 0;
        float mouseCoordsX_NDC = Input::GetMouseCoordsNDC().x;
        mouseCoordsX_NDC = p_label->rectTransform->FromGlobalNDCToLocalNDC(
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
        m_cursorIndex = closestCharRectIndex;

        // Move the selection index accordingly
        if (Input::GetMouseButtonDown(Input::MouseButton::Left) &&
            !IsShiftPressed())
        {
           ResetSelection();
        }
        if (!m_selectingWithMouse) { ResetSelection(); }
    }

    if (Input::GetMouseButtonDoubleClick(Input::MouseButton::Left))
    {
        // SelectAll();
    }
}

void UIInputText::SetCursorPosition(int index)
{
    m_cursorIndex = index;
    UpdateCursorRenderersAndScrolling();
}

void UIInputText::SetSelection(int selectionBeginIndex,
                                int selectionEndIndex)
{
    m_cursorIndex = selectionBeginIndex;
    m_selectionIndex = selectionEndIndex;
    UpdateCursorRenderersAndScrolling();
}

int UIInputText::GetSelectionBeginIndex() const
{
    return Math::Min(m_cursorIndex, m_selectionIndex);
}

int UIInputText::GetSelectionEndIndex() const
{
    return Math::Max(m_cursorIndex, m_selectionIndex);
}

String UIInputText::GetSelectedText() const
{
    if (m_cursorIndex == m_selectionIndex) { return ""; }
    return GetText()->GetContent().SubString(GetSelectionBeginIndex(),
                                             GetSelectionEndIndex());
}

void UIInputText::ResetSelection()
{
    m_selectionIndex = m_cursorIndex;
}

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
    return p_label->GetComponentInChildren<UITextRenderer>();
}

UIImageRenderer *UIInputText::GetBackground() const
{
    return p_background;
}

void UIInputText::RetrieveReferences()
{
    GameObject *go = GetGameObject(); ENSURE(go);
    p_background = go->GetComponent<UIImageRenderer>();
    p_label = SCAST<UIGameObject*>(go->FindInChildren("GUIInputText_Label"));
    p_cursor = SCAST<UITextCursor*>(go->FindInChildren("GUIInputText_GUITextCursor"));
    p_selectionQuad = SCAST<UIGameObject*>(go->FindInChildren("GUIInputText_SelectionQuad"));
    p_boxScrollArea = SCAST<UIScrollArea*>(go->FindInChildren("GUIInputText_BoxMask")->
                                            GetComponent<UIScrollArea>());
}

bool UIInputText::IsShiftPressed() const
{
    return Input::GetKey(Input::Key::LShift) ||
            Input::GetKey(Input::Key::RShift);
}

UIGameObject *UIInputText::CreateGameObject()
{
    UIGameObject *go = new UIGameObject();

    UIImageRenderer *imgRenderer = go->AddComponent<UIImageRenderer>();
    imgRenderer->UseMaterialCopy();
    imgRenderer->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    UIGameObject *label = GameObjectFactory::CreateGUILabel();
    label->SetName("GUIInputText_Label");
    label->SetParent(go);

    UIGameObject *selectionQuad = new UIGameObject("GUIInputText_SelectionQuad");
    selectionQuad->SetParent(label, 0);

    UIGameObject *boxScrollArea = GameObjectFactory::CreateGUIScrollArea();
    boxScrollArea->SetName("GUIInputText_BoxMask");
    boxScrollArea->SetParent(go);

    UITextCursor *cursor = new UITextCursor();
    cursor->SetName("GUIInputText_GUITextCursor");
    cursor->SetParent(label);

    UIInputText *inputText = new UIInputText();
    go->AddComponent(inputText);
    go->SetDefaultFocusAction(FocusAction::TakeIt);

    inputText->InitGameObject();

    return go;
}

void UIInputText::InitGameObject()
{
    RetrieveReferences();

    p_background->UseMaterialCopy();
    p_background->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    p_label->GetComponentInChildren<UIMask>()->SetMasking(false);
    p_label->rectTransform->SetMargins(5, 2, 5, 2);

    UIImageRenderer *selectionImg =
            p_selectionQuad->AddComponent<UIImageRenderer>();
    selectionImg->UseMaterialCopy();
    selectionImg->GetMaterial()->SetDiffuseColor(Color::LightBlue);
    p_selectionQuad->SetParent(p_label, 0);

    p_boxScrollArea->SetMasking(true);
    p_label->SetParent(p_boxScrollArea->GetContainer());

    GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    GetText()->SetVerticalAlign(VerticalAlignment::Center);
    GetText()->SetWrapping(false);

    m_cursorIndex = GetText()->GetContent().Size();

    ResetSelection();
    GetBackground()->SetTint(Color::White);
    GetText()->RefreshMesh();
    UpdateCursorRenderersAndScrolling();
}

float UIInputText::GetCursorXLocalNDC(int cursorIndex) const
{    
    // In case we are between two characters
    const int textLength = GetText()->GetContent().Size();
    if (cursorIndex > 0 && cursorIndex < textLength)
    {
        Rect charRect = GetText()->GetCharRectLocalNDC(cursorIndex - 1);
        float currentX = charRect.GetMax().x;
        float nextX;
        if (cursorIndex < GetText()->GetContent().Size())
        {
            Rect nextCharRect = GetText()->GetCharRectLocalNDC(cursorIndex);
            nextX = nextCharRect.GetMin().x;
        }
        else { nextX = currentX; }
        return (currentX + nextX) * 0.5f;
    }

    // In case we are at the beginning or at the end of the text
    UIGameObject *uiGo = SCAST<UIGameObject*>(p_boxScrollArea->GetGameObject());
    RectTransform *contRT = uiGo->rectTransform;
    if (!GetText()->GetCharRectsLocalNDC().IsEmpty())
    {
        if (cursorIndex == 0)
        {
            return GetText()->GetCharRectsLocalNDC().Front().GetMin().x;
        }
        return GetText()->GetCharRectsLocalNDC().Back().GetMax().x;
    }

    if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Left)
        return contRT->FromLocalNDCToGlobalNDC( Vector2(-1.0f) ).x;

    if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Center)
        return contRT->FromLocalNDCToGlobalNDC( Vector2(0.0f) ).x;

    return contRT->FromLocalNDCToGlobalNDC( Vector2(1.0f) ).x;
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
    UpdateCursorRenderersAndScrolling();
}
