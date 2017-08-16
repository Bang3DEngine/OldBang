#include "Bang/GUIInputText.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Material.h"
#include "Bang/GUILabel.h"
#include "Bang/GUIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/SingleLineRenderer.h"

GUIInputText::GUIInputText() : UIGameObject("GUIInputText")
{
    p_background = AddComponent<UIImageRenderer>();
    p_background->UseMaterialCopy();
    p_background->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    p_selectionGO = new UIGameObject("UIInputText_SelectionQuad");
    p_selectionGO->SetParent(this);
    p_selectionGO->rectTransform->SetAnchors( Vector2(-1), Vector2(1) );

    p_selectionQuad = p_selectionGO->AddComponent<UIImageRenderer>();
    p_selectionQuad->UseMaterialCopy();
    p_selectionQuad->GetMaterial()->SetDiffuseColor(Color::LightBlue);

    m_textContainer = new UIGameObject("UIInputText_Container");
    m_textContainer->rectTransform->SetAnchors( Vector2(-1), Vector2(1) );
    m_textContainer->rectTransform->SetMargins(10, 4, 4, 10);
    m_textContainer->SetParent(this);

    m_label = new GUILabel();
    m_label->GetScrollArea()->SetMasking(true);
    m_label->SetParent(m_textContainer);

    GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    GetText()->SetVerticalAlign(VerticalAlignment::Center);
    GetText()->SetWrapping(false);

    m_cursorRenderer = m_textContainer->AddComponent<SingleLineRenderer>();
    m_cursorRenderer->UseMaterialCopy();
    m_cursorRenderer->SetEnabled(false);
    m_cursorRenderer->GetMaterial()->SetDiffuseColor(Color::Black);
    m_cursorRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    m_cursorRenderer->SetRenderPass(RenderPass::Canvas);

    m_cursorIndex = GetText()->GetContent().Size();

    ResetSelection();
    SetCursorWidth(2.0f);
    GetBackground()->SetTint(Color::White);
    SetDefaultFocusAction(FocusAction::TakeIt);
    Refresh();
}

GUIInputText::~GUIInputText()
{
}

void GUIInputText::OnUpdate()
{
    GameObject::OnUpdate();

    GetText()->RefreshMesh();
    if (HasFocus())
    {
        const bool wasSelecting = (m_selectionCursorIndex != m_cursorIndex);
        HandleTyping();
        HandleCursorIndices(wasSelecting);
        Refresh();
    }
    else
    {
        m_cursorRenderer->SetEnabled(false);
    }
}


void GUIInputText::Refresh()
{
    GetText()->RefreshMesh();
    HandleTextScrolling();
    UpdateCursorRenderers();
}

void GUIInputText::UpdateCursorRenderers()
{
    Rect limitsRect = m_textContainer->rectTransform->GetScreenSpaceRectNDC();
    float minX = limitsRect.GetMin().x;
    float maxX = limitsRect.GetMax().x;
    if (!GetText()->GetContent().IsEmpty())
    {
        minX = Math::Max(minX, GetText()->GetCharRectsNDC().Front().GetMin().x);
        maxX = Math::Min(maxX, GetText()->GetCharRectsNDC().Back().GetMax().x);
    }

    // Cursor "I" position update
    float cursorX = Math::Clamp( GetCursorX_NDC(m_cursorIndex), minX, maxX );
    if (m_latestCursorX != cursorX)
    {
        Rect contentNDCRect = GetText()->GetContentNDCRect();
        Vector2 minPoint(cursorX, contentNDCRect.GetMin().y);
        Vector2 maxPoint(cursorX, contentNDCRect.GetMax().y);
        minPoint = m_textContainer->rectTransform->ToLocalNDC(minPoint);
        maxPoint = m_textContainer->rectTransform->ToLocalNDC(maxPoint);

        m_cursorRenderer->SetOrigin(  Vector3(minPoint, 0) );
        m_cursorRenderer->SetDestiny( Vector3(maxPoint, 0) );

        m_latestCursorX = cursorX;
    }

    // Selection quad rendering, depending on the selection indices
    {
        float selectionX = Math::Clamp( GetCursorX_NDC(m_selectionCursorIndex),
                                        minX, maxX);

        Rect totalLimitsRect = rectTransform->GetScreenSpaceRectNDC();
        Vector2 p1(cursorX,    totalLimitsRect.GetMin().y);
        Vector2 p2(selectionX, totalLimitsRect.GetMax().y);
        p1 = rectTransform->ToLocalNDC(p1);
        p2 = rectTransform->ToLocalNDC(p2);

        p_selectionGO->rectTransform->SetAnchorMin( Vector2::Min(p1, p2) );
        p_selectionGO->rectTransform->SetAnchorMax( Vector2::Max(p1, p2) );
    }

    m_cursorTime += Time::deltaTime;
    m_cursorRenderer->SetEnabled( m_cursorTime <= m_cursorTickTime);
    if (m_cursorTime >= m_cursorTickTime * 2) { m_cursorTime = 0.0f; }
}

void GUIInputText::HandleTyping()
{
    String content = GetText()->GetContent();
    String inputText = Input::PollInputText();

    // First we handle text deletion
    if (!GetText()->GetContent().IsEmpty())
    {
        bool selecting = (m_cursorIndex != m_selectionCursorIndex);
        int minIndex = Math::Min(m_cursorIndex, m_selectionCursorIndex) +
                       (selecting ? 1 : 0);
        int maxIndex = Math::Max(m_cursorIndex, m_selectionCursorIndex);

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
            content.Erase(minIndex, maxIndex);
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

    GetText()->SetContent(content);
    GetText()->RefreshMesh();
}

void GUIInputText::HandleTextScrolling()
{
    if (GetText()->GetContent().IsEmpty())
    {
        // GetText()->SetScrollingPx( Vector2i::Zero );
        GetText()->RefreshMesh();
        return;
    }

    // If we have the cursorIndex on non-visible char, scroll the text so that
    // the charRect is visible
    Rect limits = m_textContainer->rectTransform->GetScreenSpaceRectNDC();
    float cursorX = GetCursorX_NDC(m_cursorIndex);
    Rect bounds (limits.GetMin(), limits.GetMax());
    float boundsRight = bounds.GetMax().x;
    float boundsLeft  = bounds.GetMin().x;
    if ((cursorX < boundsLeft) || (cursorX > boundsRight))
    {
        float pivotXNDC = (cursorX > boundsRight) ? boundsRight : boundsLeft;
        Vector2 scrollNDC(cursorX - pivotXNDC, 0);
        Vector2i scrollPx = RectTransform::FromGlobalNDCToPixels(scrollNDC);
        m_label->GetScrollArea()->SetScrolling(
                    m_label->GetScrollArea()->GetScrolling() - scrollPx);
    }

    // Handle scrolling when deleting too long text
    float minCharX = GetText()->GetCharRectsNDC().Front().GetMin().x;
    float maxCharX = GetText()->GetCharRectsNDC().Back().GetMax().x;
    if ((maxCharX - minCharX) > (bounds.GetWidth()) &&
         minCharX < boundsLeft &&
         maxCharX <= boundsRight)
    {
        Vector2 scrollNDC(maxCharX - boundsRight, 0);
        Vector2i scrollPx = RectTransform::FromGlobalNDCToPixels(scrollNDC) +
                            Vector2i(3,0);
        m_label->GetScrollArea()->SetScrolling(
                    m_label->GetScrollArea()->GetScrolling() - scrollPx);
    }
    else if ((maxCharX - minCharX) < (bounds.GetWidth()))
    {
        m_label->GetScrollArea()->SetScrolling(Vector2i::Zero);
    }

    GetText()->RefreshMesh();
}

void GUIInputText::HandleCursorIndices(bool wasSelecting)
{
    // Here we will move the selection indices either by mouse or arrows...
    if (IsMouseOver() && Input::GetMouseButtonDown(Input::MouseButton::Left))
    {
        m_selectingWithMouse = true;
    }
    else if (Input::GetMouseButtonUp(Input::MouseButton::Left))
    {
        m_selectingWithMouse = false;
    }

    HandleMouseSelection();
    HandleKeySelection(wasSelecting);
}

void GUIInputText::HandleKeySelection(bool wasSelecting)
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
                  (cursorIndexAdvance == -1 &&
                   m_cursorIndex > m_selectionCursorIndex) ||
                  (cursorIndexAdvance ==  1 &&
                   m_cursorIndex < m_selectionCursorIndex);

            if (moveCursorToSelectionBoundary)
            {
                m_cursorIndex = m_selectionCursorIndex;
            }
            else { m_cursorIndex -= cursorIndexAdvance; }

            ResetSelection();
        }
    }
    if (!doingSelection && !wasSelecting) { ResetSelection(); }
}

void GUIInputText::HandleMouseSelection()
{
    // Find the closest visible char bounds to the mouse position
    if (Input::GetMouseButton(Input::MouseButton::Left))
    {
        float minDist = 2.0f;
        int closestCharRectIndex = 0;
        float mouseCoordsX_NDC = Input::GetMouseCoordsNDC().x;
        const Array<Rect>& charRectsNDC = GetText()->GetCharRectsNDC();
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
    }

    if (Input::GetMouseButtonDoubleClick(Input::MouseButton::Left))
    {
        // SelectAll();
    }
}

void GUIInputText::SetCursorPosition(int index)
{
    m_cursorIndex = index;
    Refresh();
}

void GUIInputText::SetSelection(int selectionBeginIndex,
                                int selectionEndIndex)
{
    m_cursorIndex = selectionBeginIndex;
    m_selectionCursorIndex = selectionEndIndex;
    Refresh();
}

String GUIInputText::GetSelectedText() const
{
    if (m_cursorIndex == m_selectionCursorIndex) { return ""; }
    int minIndex = Math::Min(m_cursorIndex, m_selectionCursorIndex);
    int maxIndex = Math::Max(m_cursorIndex, m_selectionCursorIndex);
    return GetText()->GetContent().SubString(minIndex, maxIndex);
}

void GUIInputText::SetCursorWidth(float cursorWidth)
{
    m_cursorRenderer->SetLineWidth(cursorWidth);
}

float GUIInputText::GetCursorWidth() const
{
    return m_cursorRenderer->GetLineWidth();
}

void GUIInputText::SetCursorTickTime(float cursorTickTime)
{
    m_cursorTickTime = cursorTickTime;
}

float GUIInputText::GetCursorTickTime() const
{
    return m_cursorTickTime;
}

void GUIInputText::ResetSelection()
{
    m_selectionCursorIndex = m_cursorIndex;
}

void GUIInputText::SelectAll()
{
    SetSelection(0, GetText()->GetContent().Size());
}

UITextRenderer *GUIInputText::GetText() const
{
    return m_label->GetText();
}

UIGameObject *GUIInputText::GetTextContainer() const
{
    return m_textContainer;
}

UIImageRenderer *GUIInputText::GetBackground() const
{
    return p_background;
}

bool GUIInputText::IsShiftPressed() const
{
    return Input::GetKey(Input::Key::LShift) ||
            Input::GetKey(Input::Key::RShift);
}

Vector2 GUIInputText::GetSideCursorMarginsNDC() const
{
    return m_textContainer->rectTransform->
                FromPixelsToLocalNDC( Vector2i(5,0) );
}

void GUIInputText::OnFocusTaken()
{
    Input::StartTextInput();
}
void GUIInputText::OnFocusLost()
{
    ResetSelection();
    Input::StopTextInput();
    m_selectingWithMouse = false;
}


float GUIInputText::GetCursorX_NDC(int cursorIndex) const
{    
    // In case we are between two characters
    const int textLength = GetText()->GetContent().Size();
    if (cursorIndex > 0 && cursorIndex < textLength)
    {
        Rect charRect = GetText()->GetCharRectNDC(cursorIndex - 1);
        float currentX = charRect.GetMax().x;
        float nextX;
        if (cursorIndex < GetText()->GetContent().Size())
        {
            Rect nextCharRect = GetText()->GetCharRectNDC(cursorIndex);
            nextX = nextCharRect.GetMin().x;
        }
        else
        {
            nextX = currentX;
        }
        return (currentX + nextX) * 0.5f;
    }

    // In case we are at the beginning or at the end of the text
    RectTransform *contRT = m_textContainer->rectTransform;
    float cursorMarginNDC = GetSideCursorMarginsNDC().x;
    if (cursorIndex == 0)
    {
        if (!GetText()->GetContent().IsEmpty())
        {
            return GetText()->GetCharRectNDC(0).GetMin().x - cursorMarginNDC;
        }

        if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Left)
            return contRT->ToGlobalNDC( Vector2(-1.0f) ).x - cursorMarginNDC;

        if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Center)
            return contRT->ToGlobalNDC( Vector2(0.0f) ).x;

        return contRT->ToGlobalNDC( Vector2(1.0f) ).x + cursorMarginNDC;
    }
    return GetText()->GetCharRectsNDC().Back().GetMax().x + cursorMarginNDC;
}

bool GUIInputText::IsDelimiter(char initialChar, char curr) const
{
    bool initialIsLetter = String::IsLetter(initialChar);
    bool differentCase   = initialIsLetter &&
                           (String::IsUpperCase(initialChar) !=
                            String::IsUpperCase(curr));
    return differentCase ||
           (!String::IsLetter(curr) && !String::IsNumber(curr));
}

int GUIInputText::GetWordSplitIndex(int startIndex, bool forward) const
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

