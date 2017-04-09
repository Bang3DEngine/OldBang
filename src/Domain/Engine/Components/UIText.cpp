#include "Bang/UIText.h"

#include "Bang/Math.h"
#include "Bang/Font.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/SelectionFramebuffer.h"
#endif

UIText::UIText() : UIRenderer()
{
    SetMesh( new Mesh() );
    SetMaterial(AssetsManager::Load<Material>("Materials/UI/G_UIText.bmat",
                                               true) );
    UseMaterialCopy();

    SetFont( AssetsManager::Load<Font>("Fonts/UbuntuFont.bfont", true) );
    SetContent("abcdef");
}

UIText::~UIText()
{
    if (m_mesh) { delete m_mesh; }
}

void UIText::RenderWithoutMaterial() const
{
    ASSERT(m_font);
    Texture2D *atlasTexture = m_font->GetAtlasTexture();
    if (atlasTexture)
    {
        UIRenderer::RenderWithoutMaterial();
    }
}

void UIText::RenderForSelectionWithoutMaterial() const
{
    RenderWithoutMaterial();
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

bool UIText::IsValidChar(char c) const
{
    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);
    return charMetrics.width > 0;
}

Vector2 UIText::GetTextSizeNDC() const
{
    constexpr float scaleFactor = 0.05f;
    return m_textSize / Screen::GetSize() * scaleFactor;
}

Rect UIText::GetNDCRectOfChar(char c) const
{
    Vector2 textSizeNDC = GetTextSizeNDC();

    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);
    Vector2 charSizeNDC (charMetrics.width, charMetrics.height);
    charSizeNDC *= textSizeNDC;

    Vector2 bearing (charMetrics.bearingX, charMetrics.bearingY);

    Vector2 charMin = -Vector2::One;
    charMin   += bearing * textSizeNDC;
    charMin.y -= (charMetrics.height - charMetrics.originY) * textSizeNDC.y;

    return Rect(charMin, charMin + charSizeNDC);
}

float UIText::GetNDCAdvance(char current, char next) const
{
    int advancePx = -1;
    if (!IsValidChar(current) || !IsValidChar(next))
    {
        advancePx = 40;
    }
    else if (m_kerning && next != '\0')
    {
        float kernX = m_font->GetKerningX(current, next);
        if (kernX > 5) { advancePx = kernX; }
    }

    if (advancePx == -1)
    {
        Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(current);
        advancePx = charMetrics.advance;
    }
    return advancePx * GetTextSizeNDC().x;
}

void UIText::FillQuadsMeshPositions()
{
    Array<Vector3> quadPos;

    float totalAdvX = 0.0f;
    Vector2 contentBoundMin = Vector2(Math::Infinity<float>());
    Vector2 contentBoundMax = Vector2(Math::NegativeInfinity<float>());
    const float hSpacingNDC = m_horizontalSpacing * GetTextSizeNDC().x;
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        if ( IsValidChar(c) )
        {
            Rect charNDCRect = GetNDCRectOfChar(c);
            Vector2 charNDCRectMin = charNDCRect.GetMin();
            Vector2 charNDCRectMax = charNDCRect.GetMax();
            charNDCRectMin += Vector2(totalAdvX, 0);
            charNDCRectMax += Vector2(totalAdvX, 0);
            contentBoundMin.x = Math::Min(contentBoundMin.x, charNDCRectMin.x);
            contentBoundMin.y = Math::Min(contentBoundMin.y, charNDCRectMin.y);
            contentBoundMax.x = Math::Max(contentBoundMax.x, charNDCRectMax.x);
            contentBoundMax.y = Math::Max(contentBoundMax.y, charNDCRectMax.y);

            quadPos.PushBack( Vector3(charNDCRectMin.x, charNDCRectMin.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMax.x, charNDCRectMin.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMax.x, charNDCRectMax.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMin.x, charNDCRectMax.y, 0) );
        }
        char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        totalAdvX += GetNDCAdvance(c, nextChar) + hSpacingNDC;
    }

    Vector2 contentSize = contentBoundMax - contentBoundMin;
    Vector2 alignDisplacement = GetAlignmentOffset(contentSize);
    for (Vector3 &pos : quadPos) { pos = pos + Vector3(alignDisplacement, 0); }

    m_mesh->LoadPositions(quadPos);
    SetRenderMode(GL::RenderMode::Quads);
}

void UIText::FillQuadsMeshUvs()
{
    ASSERT(m_font);
    Array<Vector2> quadUvs;

    for (char c : m_content)
    {
        Vector2 minUv = m_font->GetCharMinUvInAtlas(c);
        Vector2 maxUv = m_font->GetCharMaxUvInAtlas(c);
        quadUvs.PushBack( Vector2(minUv.x, maxUv.y) );
        quadUvs.PushBack( Vector2(maxUv.x, maxUv.y) );
        quadUvs.PushBack( Vector2(maxUv.x, minUv.y) );
        quadUvs.PushBack( Vector2(minUv.x, minUv.y) );
    }

    m_mesh->LoadUvs(quadUvs);
}

void UIText::RefreshMesh()
{
    ASSERT(m_font);
    SetMesh(m_mesh);
    FillQuadsMeshPositions();
    FillQuadsMeshUvs();
    if (!m_content.Contains("FPS"))
    Debug_Log(GetXMLInfo().ToString());
}

Vector2 UIText::GetAlignmentOffset(const Vector2& contentSize) const
{
    Vector2 alignOffset;
    if (m_horizontalAlignment == HorizontalAlignment::Left)
    {
        alignOffset.x = 0.0f;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Center)
    {
        alignOffset.x = -contentSize.x * 0.5f;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Right)
    {
        alignOffset.x = -contentSize.x;
    }

    if (m_verticalAlignment == VerticalAlignment::Bot)
    {
        alignOffset.y = -contentSize.y * 0.5f;
    }
    else if (m_verticalAlignment == VerticalAlignment::Center)
    {
        alignOffset.y = 0.0f;
    }
    else if (m_verticalAlignment == VerticalAlignment::Top)
    {
        alignOffset.y = contentSize.y * 0.5f;
    }

    return alignOffset;
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
    if (m_font != font)
    {
        m_font = font;
        RefreshMesh();
        GetMaterial()->SetTexture( m_font->GetAtlasTexture() );
    }
}

Font *UIText::GetFont() const
{
    return m_font;
}

void UIText::SetKerning(bool kerning)
{
    if (m_kerning != kerning)
    {
        m_kerning = kerning;
        RefreshMesh();
    }
}

bool UIText::GetKerning() const
{
    return m_kerning;
}

void UIText::SetContent(const String &content)
{
    if (m_content != content)
    {
        m_content = content;
        RefreshMesh();
    }
}

const String &UIText::GetContent() const
{
    return m_content;
}

void UIText::SetTextSize(int size)
{
    if (m_textSize != size)
    {
        m_textSize = size;
        RefreshMesh();
    }
}

int UIText::GetTextSize() const
{
    return m_textSize;
}

void UIText::SetHorizontalSpacing(int horizontalSpacing)
{
    if (m_horizontalSpacing != horizontalSpacing)
    {
        m_horizontalSpacing = horizontalSpacing;
        RefreshMesh();
    }
}

int UIText::GetHorizontalSpacing() const
{
    return m_horizontalSpacing;
}

void UIText::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);

    String fontFilepath = xmlInfo.GetFilepath("Font");
    if (!fontFilepath.Empty())
    {
        SetFont( AssetsManager::Load<Font>(fontFilepath) );
    }

    SetContent(xmlInfo.GetString("Content"));

    SetTint(xmlInfo.GetColor("Color"));
    SetTextSize(xmlInfo.GetFloat("TextSize"));
    SetHorizontalSpacing(xmlInfo.GetFloat("HSpacing"));
    SetKerning(xmlInfo.GetBool("Kerning"));

    int vAlignIndex = xmlInfo.GetEnumSelectedIndex("VerticalAlign");
    VerticalAlignment vAlign = static_cast<VerticalAlignment>(vAlignIndex);
    SetVerticalAlign(vAlign);

    int hAlignIndex = xmlInfo.GetEnumSelectedIndex("HorizontalAlign");
    HorizontalAlignment hAlign = static_cast<HorizontalAlignment>(hAlignIndex);
    SetHorizontalAlign(hAlign);

    RefreshMesh();
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
