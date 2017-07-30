#include "UIInputText.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/Material.h"
#include "Bang/RectTransform.h"
#include "Bang/SingleLineRenderer.h"

UIInputText::UIInputText() : UIGameObject("UIInputText")
{
    rectTransform->SetAnchors(Vector2(-0.6, -0.2), Vector2(0.6, 0.2));

    p_background = AddComponent<UIImage>();
    p_background->UseMaterialCopy();
    p_background->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    p_selectionGO = new UIGameObject("UIInputText_SelectionQuad");
    p_selectionGO->SetParent(this);
    p_selectionGO->rectTransform->SetAnchors( Vector2(-1), Vector2(1) );

    p_selectionQuad = p_selectionGO->AddComponent<UIImage>();
    p_selectionQuad->UseMaterialCopy();
    p_selectionQuad->GetMaterial()->SetDiffuseColor(Color::LightBlue);

    m_textContainer = new UIGameObject("UIInputText_Container");
    m_textContainer->rectTransform->SetAnchors( Vector2(-1), Vector2(1) );
    m_textContainer->SetParent(this);
    SetMargins(10, 4, 4, 10);

    p_text = m_textContainer->AddComponent<UIText>();
    p_text->SetTint(Color::Blue);
    p_text->SetTextSize(20.0f);
    p_text->SetContent("ABCDEFGHIJKLMN");
    p_text->SetHorizontalAlign(HorizontalAlignment::Left);
    p_text->SetVerticalAlign(VerticalAlignment::Center);
    p_text->SetHorizontalWrapMode(WrapMode::Hide);
    p_text->SetVerticalWrapMode(WrapMode::Hide);

    m_cursorRenderer = m_textContainer->AddComponent<SingleLineRenderer>();
    m_cursorRenderer->UseMaterialCopy();
    m_cursorRenderer->GetMaterial()->SetDiffuseColor(Color::Black);
    m_cursorRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    m_cursorRenderer->SetRenderLayer(Renderer::RenderLayer::RLCanvas);

    m_cursorIndex = p_text->GetContent().Length();
    m_selectionCursorIndex = m_cursorIndex;

    SetCursorWidth(2.0f);
    Input::StartTextInput();
}

UIInputText::~UIInputText()
{
}

void UIInputText::OnUpdate()
{
    GameObject::OnUpdate();

    const bool wasSelecting = (m_selectionCursorIndex != m_cursorIndex);

    HandleTyping();

    HandleCursorIndices(wasSelecting);
    HandleTextScrolling();

    UpdateCursorRenderers();
}

void UIInputText::UpdateCursorRenderers()
{
    Rect limitsRect = m_textContainer->rectTransform->GetScreenSpaceRectNDC();
    float minX = limitsRect.GetMin().x;
    float maxX = limitsRect.GetMax().x;
    if (!p_text->GetContent().Empty())
    {
        minX = Math::Max(minX, p_text->GetCharRectsNDC().Front().GetMin().x);
        maxX = Math::Min(maxX, p_text->GetCharRectsNDC().Back().GetMax().x);
    }

    // Cursor "I" position update, depending on the cursor index
    float cursorX = Math::Clamp( GetCursorX_NDC(m_cursorIndex), minX, maxX );
    if (m_latestCursorIndex != m_cursorIndex || m_forceUpdateRenderers)
    {
        Vector2 minPoint(cursorX, limitsRect.GetMin().y);
        Vector2 maxPoint(cursorX, limitsRect.GetMax().y);
        minPoint = m_textContainer->rectTransform->ToLocalNDC(minPoint);
        maxPoint = m_textContainer->rectTransform->ToLocalNDC(maxPoint);

        m_cursorRenderer->SetOrigin(  Vector3(minPoint, 0) );
        m_cursorRenderer->SetDestiny( Vector3(maxPoint, 0) );

        m_latestCursorIndex = m_cursorIndex;
        m_forceUpdateRenderers = false;
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

void UIInputText::HandleTyping()
{
    String content = p_text->GetContent();
    String inputText = Input::PollInputText();

    // First we handle text deletion
    if (!p_text->GetContent().Empty())
    {
        bool selecting = (m_cursorIndex != m_selectionCursorIndex);
        int minIndex = Math::Min(m_cursorIndex, m_selectionCursorIndex) +
                       (selecting ? 1 : 0);
        int maxIndex = Math::Max(m_cursorIndex, m_selectionCursorIndex);

        int offset = 1;
        if (Input::GetKeyDown(Input::Key::Delete)) { offset = (selecting ? -1 : 0); }
        else if (Input::GetKeyDown(Input::Key::BackSpace)) { offset = -1; }
        else if (selecting && !inputText.Empty()) { offset = -1; }

        minIndex += offset;
        maxIndex += offset;
        if (offset != 1 && minIndex >= 0 && maxIndex <= content.Length()-1)
        {
            content.Erase(minIndex, maxIndex);
            m_cursorIndex = minIndex;
            m_forceUpdateRenderers = true;
            ResetSelection();
        }
    }

    // Key typing handling
    if (!inputText.Empty())
    {
        content.Insert(m_cursorIndex, inputText);
        m_cursorIndex += inputText.Length();
        m_forceUpdateRenderers = true;
        ResetSelection();
    }

    p_text->SetContent(content);
}

void UIInputText::HandleTextScrolling()
{
    // If we have the cursorIndex on non-visible char, scroll the text so that
    // the charRect is visible
    RectTransform *contRT = m_textContainer->rectTransform;
    Rect limits = contRT->GetScreenSpaceRectNDC();
    float cursorX = GetCursorX_NDC(m_cursorIndex);
    Rect bounds (limits.GetMin(), limits.GetMax());
    float boundsRight = bounds.GetMax().x;
    float boundsLeft  = bounds.GetMin().x;
    if ((cursorX < boundsLeft) || (cursorX > boundsRight))
    {
        float pivotXNDC = (cursorX > boundsRight) ? boundsRight : boundsLeft;

        Vector2 scrollNDC(cursorX - pivotXNDC, 0);
        Vector2i offset( (cursorX > boundsRight) ? 5 : -5, 0 );
        Vector2i scrollPx = RectTransform::FromGlobalNDCToPixels(scrollNDC) + offset;
        p_text->SetScrollingPx(p_text->GetScrollingPx() - scrollPx);
        m_forceUpdateRenderers = true;
    }
}

void UIInputText::HandleCursorIndices(bool wasSelecting)
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

    if (m_selectingWithMouse) { HandleMouseSelection(); }
    else { HandleKeySelection(wasSelecting); }
}

void UIInputText::HandleKeySelection(bool wasSelecting)
{
    // Get cursor advance 1/-1
    int cursorIndexAdvance = 0;
    if (Input::GetKeyDown(Input::Key::Right)) { cursorIndexAdvance = 1; }
    if (Input::GetKeyDown(Input::Key::Left)) { cursorIndexAdvance = -1; }

    // Advance the the cursor index, and clamp it
    m_cursorIndex = Math::Clamp(m_cursorIndex + cursorIndexAdvance,
                                0, p_text->GetContent().Length());

    /* // No scrolling text
    while ( m_cursorIndex > 0 && !p_text->IsCharVisible(m_cursorIndex-1) )
    {
        m_cursorIndex--;
    }
    */

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

void UIInputText::HandleMouseSelection()
{
    // Find the closest visible char bounds to the mouse position
    float minDist = 2.0f;
    int closestCharRectIndex = 0;
    float mouseCoordsX_NDC = Input::GetMouseCoordsNDC().x;
    const Array<Rect>& charRectsNDC = p_text->GetCharRectsNDC();
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

    // Move the selection index accordingly
    if (Input::GetMouseButtonDown(Input::MouseButton::Left) &&
        !IsShiftPressed())
    {
        m_selectionCursorIndex = closestCharRectIndex;
    }
    m_cursorIndex = closestCharRectIndex;
}

void UIInputText::SetMargins(int left, int top, int right, int bot)
{
    RectTransform *contRT = m_textContainer->GetComponent<RectTransform>();
    contRT->SetMargins(left, top, right, bot);
}

void UIInputText::SetCursorWidth(float cursorWidth)
{
    m_cursorRenderer->SetLineWidth(cursorWidth);
}

float UIInputText::GetCursorWidth() const
{
    return m_cursorRenderer->GetLineWidth();
}

void UIInputText::SetCursorTickTime(float cursorTickTime)
{
    m_cursorTickTime = cursorTickTime;
}

float UIInputText::GetCursorTickTime() const
{
    return m_cursorTickTime;
}

void UIInputText::ResetSelection()
{
    m_selectionCursorIndex = m_cursorIndex;
}

UIText *UIInputText::GetUIText() const
{
    return p_text;
}

UIImage *UIInputText::GetBackgroundImage() const
{
    return p_background;
}

const String &UIInputText::GetContent() const
{
    return p_text->GetContent();
}

bool UIInputText::IsShiftPressed() const
{
    return Input::GetKey(Input::Key::LShift) ||
            Input::GetKey(Input::Key::RShift);
}

Vector2 UIInputText::GetSideCursorMarginsNDC() const
{
    return m_textContainer->rectTransform->
                FromPixelsToLocalNDC( Vector2i(5,0) );
}

int UIInputText::GetVisibilityFrontierCharIndex(bool right) const
{
    if (p_text->GetContent().Empty()) { return 0; }

    int firstFoundFrontier  = -1;
    int secondFoundFrontier = -1;
    for (int i = 0; i < p_text->GetContent().Length() - 1; ++i)
    {
        if (p_text->IsCharVisible(i) != p_text->IsCharVisible(i+1))
        {
            if (firstFoundFrontier < 0) { firstFoundFrontier = i; }
            else
            {
                secondFoundFrontier = i;
                break;
            }
        }
    }

    if (right)
    {
        return secondFoundFrontier >= 0 ? secondFoundFrontier :
                                          firstFoundFrontier;
    }
    return firstFoundFrontier;
}

float UIInputText::GetCursorX_NDC(int cursorIndex) const
{
    // Returns the X in global NDC, for a given cursor index

    // In case we are between two characters
    const int textLength = GetContent().Length();
    if (cursorIndex > 0 && cursorIndex < textLength)
    {
        Rect charRect = p_text->GetCharRectNDC(cursorIndex - 1);
        float currentX = charRect.GetMax().x;
        float nextX;
        if (cursorIndex < p_text->GetContent().Length())
        {
            Rect nextCharRect = p_text->GetCharRectNDC(cursorIndex);
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
        if (!p_text->GetContent().Empty())
        {
            return p_text->GetCharRectNDC(0).GetMin().x - cursorMarginNDC;
        }

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Left)
            return contRT->ToGlobalNDC( Vector2(-1.0f) ).x - cursorMarginNDC;

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Center)
            return contRT->ToGlobalNDC( Vector2(0.0f) ).x;

        return contRT->ToGlobalNDC( Vector2(1.0f) ).x + cursorMarginNDC;
    }
    return p_text->GetCharRectsNDC().Back().GetMax().x + cursorMarginNDC;
}
