#include "UIInputText.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/Material.h"
#include "Bang/RectTransform.h"
#include "Bang/SingleLineRenderer.h"

UIInputText::UIInputText() : GameObject("UIInputText")
{
    ChangeTransformByRectTransform();

    RectTransform *rt = GetComponent<RectTransform>();
    rt->SetAnchors(Vector2(-0.6, -0.2), Vector2(0.6, 0.2));

    p_background = AddComponent<UIImage>();
    p_background->UseMaterialCopy();
    p_background->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    m_textContainer = new GameObject("UIInputText_Container");
    m_textContainer->ChangeTransformByRectTransform();
    m_textContainer->SetParent(this);

    RectTransform *contRT = m_textContainer->GetComponent<RectTransform>();
    contRT->SetAnchors( Vector2(-1), Vector2(1) );
    SetMargins(10, 4, 4, 10);

    p_text = m_textContainer->AddComponent<UIText>();
    p_text->SetTint(Color::Blue);
    p_text->SetTextSize(30.0f);
    p_text->SetContent("Bang");
    p_text->SetHorizontalAlign(HorizontalAlignment::Center);
    p_text->SetVerticalAlign(VerticalAlignment::Center);
    p_text->SetHorizontalWrapMode(WrapMode::Hide);
    p_text->SetVerticalWrapMode(WrapMode::Hide);

    m_cursorRenderer = m_textContainer->AddComponent<SingleLineRenderer>();
    m_cursorRenderer->UseMaterialCopy();
    m_cursorRenderer->GetMaterial()->SetDiffuseColor(Color::Black);
    m_cursorRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    m_cursorRenderer->SetRenderLayer(Renderer::RenderLayer::RLCanvas);

    m_cursorIndex = p_text->GetContent().Length();

    SetCursorWidth(2.0f);
    Input::StartTextInput();
}

UIInputText::~UIInputText()
{
}

void UIInputText::OnUpdate()
{
    GameObject::OnUpdate();

    int cursorPosAdvance = 0;
    if (Input::GetKeyDown(Input::Key::Right)) { cursorPosAdvance = 1; }
    if (Input::GetKeyDown(Input::Key::Left)) { cursorPosAdvance = -1; }

    HandleKeys();
    m_cursorIndex = Math::Clamp(m_cursorIndex + cursorPosAdvance,
                                0, p_text->GetContent().Length());

    UpdateCursorRenderer();
}

void UIInputText::UpdateCursorRenderer()
{
    RectTransform *contRT = m_textContainer->GetComponent<RectTransform>();
    if (m_latestCursorPos != m_cursorIndex)
    {
        float currentX = GetCurrentCursorXNDC();

        Rect limitsRect = contRT->GetScreenSpaceRectNDC();
        Vector2 minPoint(currentX, limitsRect.GetMin().y);
        Vector2 maxPoint(currentX, limitsRect.GetMax().y);
        minPoint = contRT->ToGlobalNDC(minPoint);
        maxPoint = contRT->ToGlobalNDC(maxPoint);

        m_cursorRenderer->SetOrigin(  Vector3(minPoint, 0) );
        m_cursorRenderer->SetDestiny( Vector3(maxPoint, 0) );

        m_latestCursorPos = m_cursorIndex;
    }

    m_cursorTime += Time::deltaTime;
    m_cursorRenderer->SetEnabled( m_cursorTime <= m_cursorTickTime);
    if (m_cursorTime >= m_cursorTickTime * 2) { m_cursorTime = 0.0f; }
}

const String &UIInputText::GetContent() const
{
    return p_text->GetContent();
}

void UIInputText::HandleKeys()
{
    // Key typing
    String content = p_text->GetContent();
    String inputText = Input::PollInputText();
    content.Insert(m_cursorIndex, inputText);
    p_text->SetContent(content);
    m_cursorIndex += inputText.Length();

    // BackSpace case
    if (!p_text->GetContent().Empty() &&
        Input::GetKeyDown(Input::Key::BackSpace))
    {
        String content = p_text->GetContent();
        content.Erase(m_cursorIndex-1, m_cursorIndex-1);
        p_text->SetContent(content);
        m_cursorIndex--;
    }
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

UIText *UIInputText::GetUIText() const
{
    return p_text;
}

UIImage *UIInputText::GetBackgroundImage() const
{
    return p_background;
}

float UIInputText::GetCurrentCursorXNDC() const
{
    const int textLength = GetContent().Length();
    if (m_cursorIndex > 0 && m_cursorIndex < textLength)
    {
        Rect charRect = p_text->GetCharRectNDC(m_cursorIndex - 1);
        float currentX = charRect.GetMax().x;
        float nextX;
        if (m_cursorIndex < p_text->GetContent().Length())
        {
            Rect nextCharRect = p_text->GetCharRectNDC(m_cursorIndex);
            nextX = nextCharRect.GetMin().x;
        }
        else
        {
            nextX = currentX;
        }
        return (currentX + nextX) * 0.5f;
    }

    RectTransform *contRT = m_textContainer->GetComponent<RectTransform>();
    float marginNDC = contRT->ToPixelsInLocalNDC( Vector2i(5,0) ).x;
    if (m_cursorIndex == 0)
    {
        if (!p_text->GetContent().Empty())
        {
            return p_text->GetCharRectNDC(0).GetMin().x - marginNDC;
        }

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Left)
            return contRT->ToLocalNDC( Vector2(-1.0f) ).x + marginNDC;

        if (p_text->GetHorizontalAlignment() == HorizontalAlignment::Center)
            return contRT->ToLocalNDC( Vector2(0.0f) ).x;

        return contRT->ToLocalNDC( Vector2(1.0f) ).x - marginNDC;
    }
    return p_text->GetCharRectNDC(textLength - 1).GetMax().x + marginNDC;
}

