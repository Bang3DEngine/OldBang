#include "UIText.h"

#include "Math.h"
#include "Font.h"
#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Camera.h"
#include "XMLNode.h"
#include "Material.h"
#include "Transform.h"
#include "UIGameObject.h"
#include "SceneManager.h"
#include "RectTransform.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "SelectionFramebuffer.h"
#endif

UIText::UIText() : UIRenderer()
{
    m_material = AssetsManager::Load<Material>("Materials/UI/D2G_UIText.bmat",
                                               true);
    m_font = AssetsManager::Load<Font>("Fonts/UbuntuFont.bfont", true);
}

UIText::~UIText()
{
}

void UIText::Render() const
{
    glDepthFunc(GL_LEQUAL);
    RenderText();
}

void UIText::RenderForSelectionFramebufferWithoutBindingMaterial() const
{
    RenderWithoutBindingMaterial();
}

String UIText::GetName() const
{
    return "UIText";
}

void UIText::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIText *text = static_cast<UIText*>(clone);
    text->SetContent(m_content);
    text->SetTextSize(m_textSize);
    text->m_font = m_font;
    text->m_textColor = m_textColor;
}

ICloneable *UIText::Clone() const
{
    UIText *text = new UIText();
    CloneInto(text);
    return text;
}

void UIText::RenderCustomPR() const
{
    UIRenderer::RenderCustomPR();
}

void UIText::SetHorizontalAlign(UIText::HorizontalAlignment horizontalAlignment)
{
    m_horizontalAlignment = horizontalAlignment;
}

UIText::HorizontalAlignment UIText::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

void UIText::SetVerticalAlign(UIText::VerticalAlignment verticalAlignment)
{
    m_verticalAlignment = verticalAlignment;
}

UIText::VerticalAlignment UIText::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}

void UIText::SetKerning(bool kerning)
{
    m_kerning = kerning;
}

bool UIText::GetKerning() const
{
    return m_kerning;
}

void UIText::SetContent(const String &content)
{
    m_content = content;
}

const String &UIText::GetContent() const
{
    return m_content;
}

void UIText::SetTextSize(int size)
{
    m_textSize = size;
}

int UIText::GetTextSize() const
{
    return m_textSize;
}

void UIText::SetHorizontalSpacing(int horizontalSpacing)
{
    m_horizontalSpacing = horizontalSpacing;
}

int UIText::GetHorizontalSpacing() const
{
    return m_horizontalSpacing;
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

void UIText::RenderText() const
{
    ASSERT(m_font);
    ASSERT(m_material);
    ASSERT(m_material->GetShaderProgram());
    m_material->SetDiffuseColor(m_textColor);

    RectTransform *rtrans = gameObject->GetComponent<RectTransform>(); ASSERT(rtrans);
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);
    Vector2 alignOffset = GetAlignmentNDCOffset();
    Vector2 textSizeNDC = GetTextSizeNDC();

    UIGameObject *copyGo = new UIGameObject();
    RectTransform *rtransCopy = copyGo->GetComponent<RectTransform>();
    rtrans->CloneInto(rtransCopy);
    rtrans->SetMargins(0,0,0,0);

    GameObject *originalParent = gameObject->parent;
    gameObject->SetParent(nullptr);


    float totalAdvX = 0.0f;
    const float hSpacingNDC = m_horizontalSpacing * textSizeNDC.x;
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        if ( IsValidChar(c) )
        {
            Rect charNDCRect = GetNDCRectOfChar(c, screenRectNDC);
            Vector2 charNDCRectMin = charNDCRect.GetMin();
            Vector2 charNDCRectMax = charNDCRect.GetMax();
            charNDCRectMin += alignOffset + Vector2(totalAdvX, 0);
            charNDCRectMax += alignOffset + Vector2(totalAdvX, 0);

            rtrans->SetAnchors(charNDCRectMin, charNDCRectMax);

            Texture2D *charTexture = m_font->GetCharacterTexture(c);
            if (charTexture)
            {
                m_material->SetTexture(charTexture);
                UIRenderer::Render();
                m_material->SetTexture(nullptr);
            }
        }

        char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        totalAdvX += GetNDCAdvance(c, nextChar) + hSpacingNDC;
    }

    rtransCopy->CloneInto(rtrans);
    delete copyGo;
    gameObject->SetParent(originalParent);
}

bool UIText::IsValidChar(char c) const
{
    const Font::CharGlyphMetrics &charMetrics = m_font->GetCharacterMetrics(c);
    return charMetrics.width > 0;
}

Vector2 UIText::GetTextSizeNDC() const
{
    constexpr float scaleFactor = 0.05f;
    return m_textSize / Screen::GetSize() * scaleFactor;
}

Vector2 UIText::GetContentNDCSize() const
{
    const Vector2 textSizeNDC = GetTextSizeNDC();

    float minContentY = Math::Infinity<float>();
    float maxContentY = Math::NegativeInfinity<float>();
    Vector2 contentSize (0, 0);
    for (int i = 0; i < m_content.Length(); ++i)
    {
        contentSize.x += m_horizontalSpacing * textSizeNDC.x;

        char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        contentSize.x += GetNDCAdvance(m_content[i], nextChar);

        Rect charRectNDC = GetNDCRectOfChar(m_content[i]);
        minContentY = Math::Min(charRectNDC.m_miny, minContentY);
        maxContentY = Math::Max(charRectNDC.m_maxy, maxContentY);
    }
    contentSize.y = maxContentY - minContentY;
    return contentSize;
}

Rect UIText::GetNDCRectOfChar(char c) const
{
    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);
    return GetNDCRectOfChar(c, screenRectNDC);
}

Rect UIText::GetNDCRectOfChar(char c, const Rect &screenRectNDC) const
{
    Vector2 textSizeNDC = GetTextSizeNDC();

    const Font::CharGlyphMetrics &charMetrics = m_font->GetCharacterMetrics(c);
    Vector2 charSizeNDC (charMetrics.width, charMetrics.height);
    charSizeNDC *= textSizeNDC;

    Vector2 charAnchorMin = screenRectNDC.GetMin();

    // Apply Bearings(X/Y)
    Vector2 bearing (charMetrics.bearingX, charMetrics.bearingY);
    charAnchorMin   += bearing * textSizeNDC;
    charAnchorMin.y -= (charMetrics.height - charMetrics.originY) *
                        textSizeNDC.y;

    return Rect(charAnchorMin, charAnchorMin + charSizeNDC);
}

float UIText::GetNDCAdvance(char current, char next) const
{
    const Font::CharGlyphMetrics &charMetrics =
            m_font->GetCharacterMetrics(current);
    int advancePx = charMetrics.advance;
    if (next == '\0' || !IsValidChar(current) || !IsValidChar(next))
    {
        advancePx = 40;
    }
    else
    {
        if (m_kerning && next != '\0')
        {
            float kernX = m_font->GetKerningX(current, next);
            if (kernX > 5) { advancePx = kernX; }
        }
    }
    return advancePx * GetTextSizeNDC().x;
}

Vector2 UIText::GetAlignmentNDCOffset() const
{
    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    Vector2 contentSize = GetContentNDCSize();
    Vector2 screenRectNDCSize = rtrans->GetScreenSpaceRect(true).GetSize();

    Vector2 alignNDCOffset;
    if (m_horizontalAlignment == HorizontalAlignment::Left)
    {
        alignNDCOffset.x = 0.0f;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Center)
    {
        alignNDCOffset.x = contentSize.x * -0.5f +
                           screenRectNDCSize.x * 0.5f;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Right)
    {
        alignNDCOffset.x = contentSize.x * -1.0f +
                           screenRectNDCSize.x * 1.0f;
    }

    if (m_verticalAlignment == VerticalAlignment::Top)
    {
        alignNDCOffset.y = contentSize.y * -1.0f +
                           screenRectNDCSize.y * 1.0f;
    }
    else if (m_verticalAlignment == VerticalAlignment::Center)
    {
        alignNDCOffset.y = contentSize.y * -0.5f +
                           screenRectNDCSize.y * 0.5f;
    }
    else if (m_verticalAlignment == VerticalAlignment::Bot)
    {
        alignNDCOffset.y = 0.0f;
    }

    Debug_Log(contentSize);

    return alignNDCOffset;
}

void UIText::ReadXMLInfo(const XMLNode *xmlInfo)
{
    UIRenderer::ReadXMLInfo(xmlInfo);

    String fontFilepath = xmlInfo->GetFilepath("Font");
    if (!fontFilepath.Empty())
    {
        m_font = AssetsManager::Load<Font>(fontFilepath);
    }

    m_textColor = xmlInfo->GetColor("Color");
    SetContent(xmlInfo->GetString("Content"));
    SetTextSize(xmlInfo->GetFloat("TextSize"));
    SetHorizontalSpacing(xmlInfo->GetFloat("HSpacing"));
    SetKerning(xmlInfo->GetBool("Kerning"));

    int vAlignIndex = xmlInfo->GetEnumSelectedIndex("VerticalAlign");
    VerticalAlignment vAlign = static_cast<VerticalAlignment>(vAlignIndex);
    SetVerticalAlign(vAlign);

    int hAlignIndex = xmlInfo->GetEnumSelectedIndex("HorizontalAlign");
    HorizontalAlignment hAlign = static_cast<HorizontalAlignment>(hAlignIndex);
    SetHorizontalAlign(hAlign);
}

void UIText::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    xmlInfo->SetFilepath("Font", m_font ? m_font->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", m_textColor);
    xmlInfo->SetString("Content", m_content, {XMLProperty::Inline});
    xmlInfo->SetFloat("TextSize", m_textSize);
    xmlInfo->SetFloat("HSpacing", m_horizontalSpacing);
    xmlInfo->SetBool("Kerning", m_kerning);
    xmlInfo->SetEnum("VerticalAlign", {"Top", "Center", "Bot"},
                     int(m_verticalAlignment), {});
    xmlInfo->SetEnum("HorizontalAlign", {"Left", "Center", "Right"},
                     int(m_horizontalAlignment), {});

    xmlInfo->GetAttribute("Tint")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("ReceivesLighting")->SetProperty({XMLProperty::Hidden});
}
