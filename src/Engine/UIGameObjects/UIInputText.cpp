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
    p_text->SetContent("Bangerinolol");
    p_text->SetHorizontalAlign(HorizontalAlignment::Right);
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
    HandleSelectionIndices(wasSelecting);

    UpdateCursorRenderers();
}

void UIInputText::UpdateCursorRenderers()
{
    float cursorX = GetCursorX_NDC(m_cursorIndex);
    if (m_latestCursorIndex != m_cursorIndex || m_forceUpdateRenderers)
    {
        Rect limitsRect = m_textContainer->rectTransform->GetScreenSpaceRectNDC();

        Vector2 minPoint(cursorX, limitsRect.GetMin().y);
        Vector2 maxPoint(cursorX, limitsRect.GetMax().y);
        minPoint = m_textContainer->rectTransform->ToLocalNDC(minPoint);
        maxPoint = m_textContainer->rectTransform->ToLocalNDC(maxPoint);

        m_cursorRenderer->SetOrigin(  Vector3(minPoint, 0) );
        m_cursorRenderer->SetDestiny( Vector3(maxPoint, 0) );

        m_latestCursorIndex = m_cursorIndex;
        m_forceUpdateRenderers = false;
    }

    // Selection quad
    {
        Rect totalLimitsRect = rectTransform->GetScreenSpaceRectNDC();
        float selectionX = GetCursorX_NDC(m_selectionCursorIndex);

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

const String &UIInputText::GetContent() const
{
    return p_text->GetContent();
}

void UIInputText::HandleTyping()
{
    String content = p_text->GetContent();
    String inputText = Input::PollInputText();

    // Handle Deletion
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

    // Key typing
    if (!inputText.Empty())
    {
        content.Insert(m_cursorIndex, inputText);
        m_cursorIndex += inputText.Length();
        m_forceUpdateRenderers = true;
        ResetSelection();
    }

    p_text->SetContent(content);
}

void UIInputText::HandleSelectionIndices(bool wasSelecting)
{
    if (IsMouseOver() && Input::GetMouseButtonDown(Input::MouseButton::Left))
    {
        m_selectingWithMouse = true;
    }
    else if (Input::GetMouseButtonUp(Input::MouseButton::Left))
    {
        m_selectingWithMouse = false;
    }

    if (m_selectingWithMouse)
    {
        HandleMouseSelection();
    }
    else
    {
        HandleKeySelection(wasSelecting);
    }
}


void UIInputText::HandleKeySelection(bool wasSelecting)
{
    int cursorIndexAdvance = 0;
    if (Input::GetKeyDown(Input::Key::Right)) { cursorIndexAdvance = 1; }
    if (Input::GetKeyDown(Input::Key::Left)) { cursorIndexAdvance = -1; }

    m_cursorIndex = Math::Clamp(m_cursorIndex + cursorIndexAdvance,
                                0, p_text->GetContent().Length());

    // Selection indices
    bool doingSelection = Input::GetKey(Input::Key::LShift) ||
                          Input::GetKey(Input::Key::RShift) ||
                          m_selectingWithMouse;
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

    if (Input::GetMouseButtonDown(Input::MouseButton::Left))
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

float UIInputText::GetCursorX_NDC(int cursorIndex) const
{
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

    RectTransform *contRT = m_textContainer->rectTransform;
    float marginNDC = contRT->ToPixelsInLocalNDC( Vector2i(5,0) ).x;
    if (cursorIndex == 0)
    {
        if (!p_text->GetContent().Empty())
        {
            return p_text->GetCharRectNDC(0).GetMin().x - marginNDC;
        }

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Left)
            return contRT->ToGlobalNDC( Vector2(-1.0f) ).x + marginNDC;

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Center)
            return contRT->ToGlobalNDC( Vector2(0.0f) ).x;

        return contRT->ToGlobalNDC( Vector2(1.0f) ).x - marginNDC;
    }
    return p_text->GetCharRectNDC(textLength - 1).GetMax().x + marginNDC;
}

