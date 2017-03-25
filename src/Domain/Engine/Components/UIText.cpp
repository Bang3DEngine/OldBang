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
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "RectTransform.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#include "SelectionFramebuffer.h"
#endif

UIText::UIText() : UIRenderer()
{
    SetMaterial( AssetsManager::Load<Material>("Materials/UI/G_UIText.bmat",
                                               true) );
    UseMaterialCopy();

    SetFont( AssetsManager::Load<Font>("Fonts/UbuntuFont.bfont", true) );

    m_content = "Hello Bang";
}

UIText::~UIText()
{
}

void UIText::Render() const
{
    RenderText();
}

void UIText::RenderForSelectionWithoutMaterial() const
{
    RenderWithoutMaterial();
}

void UIText::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIText *text = Object::SCast<UIText>(clone);
    text->SetFont ( GetFont() );
    text->SetContent( GetContent() );
    text->SetTextSize( GetTextSize() );
    text->SetHorizontalSpacing( GetHorizontalSpacing() );
    text->SetHorizontalAlign( GetHorizontalAlignment() );
    text->SetVerticalAlign( GetVerticalAlignment() );
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

void UIText::SetFont(Font *font)
{
    m_font = font;
}

Font *UIText::GetFont() const
{
    return m_font;
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

void UIText::GetContentNDCBounds(Vector2 *min, Vector2 *max,
                                 bool applyAlign) const
{
    if (!applyAlign)
    {
        const Vector2 textSizeNDC = GetTextSizeNDC();

        float totalAdvX = 0.0f;
        min->x = min->y = Math::Infinity<float>();
        max->x = max->y = Math::NegativeInfinity<float>();
        for (int i = 0; i < m_content.Length(); ++i)
        {
            Rect charRectNDC = GetNDCRectOfChar(m_content[i]);
            min->x = Math::Min(charRectNDC.m_minx, min->x);
            min->y = Math::Min(charRectNDC.m_miny, min->y);
            max->x = Math::Max(totalAdvX + charRectNDC.m_maxx, max->x);
            max->y = Math::Max(charRectNDC.m_maxy, max->y);

            if (i >= 0 && i < m_content.Length() - 1)
            {
                totalAdvX += m_horizontalSpacing * textSizeNDC.x;
            }
            char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
            totalAdvX += GetNDCAdvance(m_content[i], nextChar);
        }
        max->x = Math::Max(max->x, min->x + totalAdvX);
    }
    else
    {
        GetContentNDCBounds(min, max, false);
        Vector2 alignOffset = GetAlignmentNDCOffset();
        *min += alignOffset; *max += alignOffset;
    }
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

void UIText::RenderText() const
{
    ASSERT(m_font);
    Material *mat = GetMaterial();
    ASSERT(mat); ASSERT(mat->GetShaderProgram());

    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);
    Vector2 alignOffset = GetAlignmentNDCOffset();
    Vector2 textSizeNDC = GetTextSizeNDC();

    GameObject *copyGo = new GameObject("Text rectTransform copyGo");
    copyGo->ChangeTransformByRectTransform();
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
            GL::SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
            Rect charNDCRect = GetNDCRectOfChar(c, screenRectNDC);
            Vector2 charNDCRectMin = charNDCRect.GetMin();
            Vector2 charNDCRectMax = charNDCRect.GetMax();
            charNDCRectMin += alignOffset + Vector2(totalAdvX, 0);
            charNDCRectMax += alignOffset + Vector2(totalAdvX, 0);

            rtrans->SetAnchors(charNDCRectMin, charNDCRectMax);

            Texture2D *charTexture = m_font->GetCharacterTexture(c);
            if (charTexture)
            {
                mat->SetTexture(charTexture);
                UIRenderer::Render();
                mat->SetTexture(nullptr);
            }
        }
        char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        totalAdvX += GetNDCAdvance(c, nextChar) + hSpacingNDC;
    }

    GL::SetViewProjMode(GL::ViewProjMode::UseBoth);

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

Rect UIText::GetNDCRectOfChar(char c) const
{
    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    ASSERT(rtrans, "", return Rect::Empty);
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
    if (!IsValidChar(current) || !IsValidChar(next))
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
    Vector2 contentMin, contentMax;
    GetContentNDCBounds(&contentMin, &contentMax, false);
    Vector2 contentCenter = (contentMax + contentMin) * 0.5f;
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);

    Vector2 alignNDCOffset;
    if (m_horizontalAlignment == HorizontalAlignment::Left)
    {
        alignNDCOffset.x = (screenRectNDC.GetMin() - contentMin).x;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Center)
    {
        alignNDCOffset.x = (screenRectNDC.GetCenter() - contentCenter).x;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Right)
    {
        alignNDCOffset.x = (screenRectNDC.GetMax() - contentMax).x;
    }


    if (m_verticalAlignment == VerticalAlignment::Top)
    {
        alignNDCOffset.y = (screenRectNDC.GetMax() - contentMax).y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Center)
    {
        alignNDCOffset.y = (screenRectNDC.GetCenter() - contentCenter).y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Bot)
    {
        alignNDCOffset.y = (screenRectNDC.GetMin() - contentMin).y;
    }

    return alignNDCOffset;
}

void UIText::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);

    String fontFilepath = xmlInfo.GetFilepath("Font");
    if (!fontFilepath.Empty())
    {
        m_font = AssetsManager::Load<Font>(fontFilepath);
    }

    m_tint = xmlInfo.GetColor("Color");
    SetContent(xmlInfo.GetString("Content"));
    SetTextSize(xmlInfo.GetFloat("TextSize"));
    SetHorizontalSpacing(xmlInfo.GetFloat("HSpacing"));
    SetKerning(xmlInfo.GetBool("Kerning"));

    int vAlignIndex = xmlInfo.GetEnumSelectedIndex("VerticalAlign");
    VerticalAlignment vAlign = static_cast<VerticalAlignment>(vAlignIndex);
    SetVerticalAlign(vAlign);

    int hAlignIndex = xmlInfo.GetEnumSelectedIndex("HorizontalAlign");
    HorizontalAlignment hAlign = static_cast<HorizontalAlignment>(hAlignIndex);
    SetHorizontalAlign(hAlign);
}

void UIText::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    xmlInfo->SetFilepath("Font", m_font ? m_font->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", m_tint);
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
}
