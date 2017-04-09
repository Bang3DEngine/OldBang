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
#include "Bang/RectTransform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
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
    SetContent("");
    RefreshMesh();
}

UIText::~UIText()
{
    if (m_mesh) { delete m_mesh; }
}

void UIText::RenderWithoutMaterial() const
{
    ASSERT(m_font);

    ASSERT(gameObject);
    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    Rect ndcRect = rtrans->GetScreenSpaceRect(true);

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

void UIText::GetContentNDCBounds(Vector2 *min, Vector2 *max,
                                 bool applyAlign) const
{
    if (!applyAlign)
    {
        float totalAdvX = 0.0f;
        *min = Vector2(Math::Infinity<float>());
        *max = Vector2(Math::NegativeInfinity<float>());
        for (int i = 0; i < m_content.Length(); ++i)
        {
            Rect charRectNDC = GetNDCRectOfChar(m_content[i]);
            min->x = Math::Min(charRectNDC.GetMin().x, min->x);
            min->y = Math::Min(charRectNDC.GetMin().y, min->y);
            max->x = Math::Max(totalAdvX + charRectNDC.GetMax().x, max->x);
            max->y = Math::Max(            charRectNDC.GetMax().y, max->y);

            Debug::DrawScreenLine(charRectNDC.GetMin(), charRectNDC.GetMax(),
                                  Color::Red);

            if (i >= 0 && i < m_content.Length() - 1)
            {
                totalAdvX += m_horizontalSpacing * GetTextSizeNDC().x;
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
    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
	if (!rtrans) { return Rect::Empty; }
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);
    return GetNDCRectOfChar(c, screenRectNDC);
}

Rect UIText::GetNDCRectOfChar(char c, const Rect &screenRectNDC) const
{
    Vector2 textSizeNDC = GetTextSizeNDC();

    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);
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
    const float hSpacingNDC = m_horizontalSpacing * GetTextSizeNDC().x;
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        if ( IsValidChar(c) )
        {
            Rect charNDCRect = GetNDCRectOfChar(c, Rect::ScreenRect);
            Vector2 charNDCRectMin = charNDCRect.GetMin();
            Vector2 charNDCRectMax = charNDCRect.GetMax();
            charNDCRectMin += Vector2(totalAdvX, 0);
            charNDCRectMax += Vector2(totalAdvX, 0);

            quadPos.PushBack( Vector3(charNDCRectMin.x, charNDCRectMin.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMax.x, charNDCRectMin.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMax.x, charNDCRectMax.y, 0) );
            quadPos.PushBack( Vector3(charNDCRectMin.x, charNDCRectMax.y, 0) );
        }
        char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        totalAdvX += GetNDCAdvance(c, nextChar) + hSpacingNDC;
    }

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
    FillQuadsMeshPositions();
    FillQuadsMeshUvs();
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

    m_tint = xmlInfo.GetColor("Color");
    SetTextSize(xmlInfo.GetFloat("TextSize"));
    SetHorizontalSpacing(xmlInfo.GetFloat("HSpacing"));
    SetKerning(xmlInfo.GetBool("Kerning"));

    int vAlignIndex = xmlInfo.GetEnumSelectedIndex("VerticalAlign");
    VerticalAlignment vAlign = static_cast<VerticalAlignment>(vAlignIndex);
    SetVerticalAlign(vAlign);

    int hAlignIndex = xmlInfo.GetEnumSelectedIndex("HorizontalAlign");
    HorizontalAlignment hAlign = static_cast<HorizontalAlignment>(hAlignIndex);
    SetHorizontalAlign(hAlign);

    SetContent(xmlInfo.GetString("Content")); // Order matters
    RefreshMesh();
    SetMesh(m_mesh);
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
