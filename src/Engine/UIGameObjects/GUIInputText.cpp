#include "Bang/GUIInputText.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/GUIMask.h"
#include "Bang/Material.h"
#include "Bang/GUILabel.h"
#include "Bang/GUIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/SingleLineRenderer.h"

const Vector2i GUIInputText::LookAheadOffsetPx = Vector2i(5);

GUIInputText::GUIInputText() : UIGameObject("GUIInputText")
{
    p_background = AddComponent<UIImageRenderer>();
    p_background->UseMaterialCopy();
    p_background->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    m_label = new GUILabel();
    m_label->GetMask()->SetMasking(false);

    p_selectionQuad = new UIGameObject("GUIInputText_SelectionQuad");
    UIImageRenderer *selectionImg =
            p_selectionQuad->AddComponent<UIImageRenderer>();
    selectionImg->UseMaterialCopy();
    selectionImg->GetMaterial()->SetDiffuseColor(Color::LightBlue);
    p_selectionQuad->SetParent(m_label, 0);

    m_boxScrollArea = new GUIScrollArea();
    m_boxScrollArea->SetName("GUIInputText_BoxMask");
    m_boxScrollArea->AddChild(m_label);
    m_boxScrollArea->SetParent(this);

    GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    GetText()->SetVerticalAlign(VerticalAlignment::Center);
    GetText()->SetWrapping(false);

    m_cursorRenderer = m_label->AddComponent<SingleLineRenderer>();
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
    GetText()->RefreshMesh();
    UpdateCursorRenderersAndScrolling();
}

GUIInputText::~GUIInputText()
{
}

void GUIInputText::OnUpdate()
{
    GameObject::OnUpdate();

    if (HasFocus())
    {
        const bool wasSelecting = (m_selectionIndex != m_cursorIndex);
        HandleTyping();
        HandleCursorIndices(wasSelecting);
        UpdateCursorRenderersAndScrolling();
    }
    else
    {
        m_cursorRenderer->SetEnabled(false);
    }
}

void GUIInputText::UpdateCursorRenderersAndScrolling()
{
    RectTransform *rt = rectTransform;
    Rect limits = rt->GetScreenSpaceRectNDC();

    Vector2i prevScrolling = m_boxScrollArea->GetScrolling();
    m_boxScrollArea->SetScrolling( Vector2i::Zero ); // To make things easier

    // Cursor "I" position update and Selection quad rendering
    {
        float cursorX = GetCursorXGlobalNDC(m_cursorIndex);
        {
            Rect contentGlobalNDCRect = GetText()->GetContentGlobalNDCRect();
            Vector2 minPoint(cursorX, contentGlobalNDCRect.GetMin().y);
            Vector2 maxPoint(cursorX, contentGlobalNDCRect.GetMax().y);
            minPoint = rt->FromGlobalNDCToLocalNDC(minPoint);
            maxPoint = rt->FromGlobalNDCToLocalNDC(maxPoint);
            m_cursorRenderer->SetOrigin(  Vector3(minPoint, 0) );
            m_cursorRenderer->SetDestiny( Vector3(maxPoint, 0) );
        }

        // Selection quad
        {
            float selectionX = GetCursorXGlobalNDC(m_selectionIndex);
            Vector2 p1(cursorX,    limits.GetMin().y);
            Vector2 p2(selectionX, limits.GetMax().y);
            p1 = rt->FromGlobalNDCToLocalNDC(p1);
            p2 = rt->FromGlobalNDCToLocalNDC(p2);

            p_selectionQuad->rectTransform->SetAnchorMin( Vector2::Min(p1, p2) );
            p_selectionQuad->rectTransform->SetAnchorMax( Vector2::Max(p1, p2) );
        }
    }

    // Text Scrolling
    {
        Vector2 scrollNDC = Vector2::Zero;
        Rect contentRectNDC = GetText()->GetContentGlobalNDCRect();
        if (contentRectNDC.GetWidth() < limits.GetWidth() || m_cursorIndex == 0)
        {
            m_boxScrollArea->SetScrolling( Vector2i::Zero );
        }
        else
        {
            m_boxScrollArea->SetScrolling(prevScrolling);
            float cursorX = GetCursorXGlobalNDC(m_cursorIndex);
            float lookAheadNDC =
                RectTransform::FromPixelsAmountToGlobalNDC(LookAheadOffsetPx).x;
            if (cursorX < limits.GetMin().x)
            {
                scrollNDC.x = limits.GetMin().x - cursorX + lookAheadNDC;
            }
            else if (cursorX > limits.GetMax().x)
            {
                scrollNDC.x = limits.GetMax().x - cursorX - lookAheadNDC;
            }
            else
            {
                if (contentRectNDC.GetMin().x < limits.GetMin().x &&
                    contentRectNDC.GetMax().x < limits.GetMax().x)
                {
                    scrollNDC.x = limits.GetMax().x - contentRectNDC.GetMax().x;
                }
            }
            Vector2i scrollPx = RectTransform::FromGlobalNDCToPixelsAmount(scrollNDC);
            m_boxScrollArea->SetScrolling(prevScrolling + scrollPx);
        }
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

    if (content != GetText()->GetContent())
    {
        GetText()->SetContent(content);
        GetText()->RefreshMesh();
    }
}

void GUIInputText::HandleCursorIndices(bool wasSelecting)
{
    // Here we will move the selection indices either by mouse or arrows...
    if (IsMouseOver() && Input::GetMouseButtonDown(Input::MouseButton::Left))
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

float GUIInputText::GetCursorXGlobalNDC(int cursorIndex) const
{
    return m_label->rectTransform->FromLocalNDCToGlobalNDC(
                            Vector2(GetCursorXLocalNDC(cursorIndex), 0) ).x;
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

void GUIInputText::HandleMouseSelection()
{
    // Find the closest visible char bounds to the mouse position
    if (Input::GetMouseButton(Input::MouseButton::Left))
    {
        float minDist = 2.0f;
        int closestCharRectIndex = 0;
        float mouseCoordsX_NDC = Input::GetMouseCoordsNDC().x;
        mouseCoordsX_NDC = m_label->rectTransform->FromGlobalNDCToLocalNDC(
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

void GUIInputText::SetCursorPosition(int index)
{
    m_cursorIndex = index;
    UpdateCursorRenderersAndScrolling();
}

void GUIInputText::SetSelection(int selectionBeginIndex,
                                int selectionEndIndex)
{
    m_cursorIndex = selectionBeginIndex;
    m_selectionIndex = selectionEndIndex;
    UpdateCursorRenderersAndScrolling();
}

String GUIInputText::GetSelectedText() const
{
    if (m_cursorIndex == m_selectionIndex) { return ""; }
    int minIndex = Math::Min(m_cursorIndex, m_selectionIndex);
    int maxIndex = Math::Max(m_cursorIndex, m_selectionIndex);
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
    m_selectionIndex = m_cursorIndex;
}

void GUIInputText::SelectAll()
{
    SetSelection(0, GetText()->GetContent().Size());
}

UITextRenderer *GUIInputText::GetText() const
{
    return m_label->GetText();
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


void GUIInputText::OnFocusTaken()
{
    Input::StartTextInput();
}
void GUIInputText::OnFocusLost()
{
    ResetSelection();
    Input::StopTextInput();
    m_selectingWithMouse = false;
    UpdateCursorRenderersAndScrolling();
}


float GUIInputText::GetCursorXLocalNDC(int cursorIndex) const
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
    RectTransform *contRT = m_boxScrollArea->rectTransform;
    if (cursorIndex == 0)
    {
        if (!GetText()->GetContent().IsEmpty())
        {
            return GetText()->GetCharRectLocalNDC(0).GetMin().x;
        }

        if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Left)
            return contRT->FromLocalNDCToGlobalNDC( Vector2(-1.0f) ).x;

        if (GetText()->GetHorizontalAlignment() == HorizontalAlignment::Center)
            return contRT->FromLocalNDCToGlobalNDC( Vector2(0.0f) ).x;

        return contRT->FromLocalNDCToGlobalNDC( Vector2(1.0f) ).x;
    }
    return GetText()->GetCharRectsLocalNDC().Back().GetMax().x;
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

