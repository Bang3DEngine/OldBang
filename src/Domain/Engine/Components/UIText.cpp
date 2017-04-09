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
    SetContent("");

    SetRenderMode(GL::RenderMode::Quads);
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

Rect UIText::GetCharRect(char c) const
{
    Vector2 textSize = GetTextSizeScaled();

    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);
    Vector2 charSize (charMetrics.width, charMetrics.height);
    charSize *= textSize;

    Vector2 bearing (charMetrics.bearingX, charMetrics.bearingY);

    Vector2 charMin = -Vector2::One;
    charMin   += bearing * textSize;
    charMin.y -= (charMetrics.height - charMetrics.originY) * textSize.y;

    return Rect(charMin, charMin + charSize);
}

float UIText::GetNDCAdvance(char current, char next) const
{
    int advancePx = -1;

    // TODO: Kerning
    // if (m_kerning && next != '\0')
    //{
    //    float kernX = m_font->GetKerningX(current, next);
    //    if (kernX > 5) { advancePx = kernX; }
    //}

    if ( !IsValidChar(current) ) { advancePx = 100; }
    if (advancePx < 0)
    {
        Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(current);
        advancePx = charMetrics.advance;
    }
    return advancePx * GetTextSizeScaled().x;
}

void UIText::FillQuadsMeshPositions()
{
    Array<Vector3> quadPos;

    float totalAdv = 0.0f;
    Vector2 contentBoundMin = Vector2(Math::Infinity<float>());
    Vector2 contentBoundMax = Vector2(Math::NegativeInfinity<float>());
    const float hSpacingNDC = m_horizontalSpacing * GetTextSizeScaled().x;
    for (int i = 0; i < m_content.Length(); ++i)
    {
        const char c = m_content[i];

        Rect charRect = GetCharRect(c);
        Vector2 charRectMin = charRect.GetMin() + Vector2::Right * totalAdv;
        Vector2 charRectMax = charRect.GetMax() + Vector2::Right * totalAdv;
        contentBoundMin.x = Math::Min(contentBoundMin.x, charRectMin.x);
        contentBoundMin.y = Math::Min(contentBoundMin.y, charRectMin.y);
        contentBoundMax.x = Math::Max(contentBoundMax.x, charRectMax.x);
        contentBoundMax.y = Math::Max(contentBoundMax.y, charRectMax.y);

        quadPos.PushBack( Vector3(charRectMin.x, charRectMin.y, 0) );
        quadPos.PushBack( Vector3(charRectMax.x, charRectMin.y, 0) );
        quadPos.PushBack( Vector3(charRectMax.x, charRectMax.y, 0) );
        quadPos.PushBack( Vector3(charRectMin.x, charRectMax.y, 0) );

        const char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        const float advance = GetNDCAdvance(c, nextChar);
        totalAdv += advance + hSpacingNDC;
    }
    contentBoundMax.x = Math::Max(contentBoundMax.x,
                                  contentBoundMin.x + totalAdv);

    Vector2 contentSize = contentBoundMax - contentBoundMin;
    for (Vector3 &pos : quadPos)
    {
        Vector2 pos2 = pos.xy();
        ApplyAlignmentOffset(contentSize, &pos2);
        pos = Vector3(pos2, pos.z);
    }

    m_mesh->LoadPositions(quadPos);
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
}

Vector2 UIText::ApplyAlignmentOffset(const Vector2& contentSize,
                                     Vector2 *minPosition) const
{
    if (m_horizontalAlignment == HorizontalAlignment::Center)
    {
        minPosition->x = minPosition->x - contentSize.x * 0.5f + 1.0f;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Right)
    {
        minPosition->x = minPosition->x - contentSize.x + 2.0f;
    }

    if (m_verticalAlignment == VerticalAlignment::Center)
    {
        minPosition->y = minPosition->y - contentSize.y * 0.5f + 1.0f;
    }
    else if (m_verticalAlignment == VerticalAlignment::Top)
    {
        minPosition->y = minPosition->y - contentSize.y + 2.0f;
    }
}

void UIText::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);

    UIText *text = Object::SCast<UIText>(clone);
    text->SetMesh( new Mesh() );
    text->SetFont ( GetFont() );
    text->SetContent( GetContent() );
    text->SetTextSize( GetTextSize() );
    text->SetHorizontalSpacing( GetHorizontalSpacing() );
    text->SetHorizontalAlign( GetHorizontalAlignment() );
    text->SetVerticalAlign( GetVerticalAlignment() );
    text->RefreshMesh();
}

void UIText::SetHorizontalAlign(UIText::HorizontalAlignment horizontalAlignment)
{
    if (m_horizontalAlignment != horizontalAlignment)
    {
        m_horizontalAlignment = horizontalAlignment;
        RefreshMesh();
    }
}

UIText::HorizontalAlignment UIText::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

void UIText::SetVerticalAlign(UIText::VerticalAlignment verticalAlignment)
{
    if (m_verticalAlignment != verticalAlignment)
    {
        m_verticalAlignment = verticalAlignment;
        RefreshMesh();
    }
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

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

bool UIText::IsValidChar(char c) const
{
    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);
    return c != ' ' && charMetrics.width > 0;
}

Vector2 UIText::GetTextSizeScaled() const
{
    constexpr double scaleFactor = 0.00015;
    return Vector2(m_textSize * scaleFactor);
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

    xmlInfo->SetFilepath("Font", GetFont() ?
                             GetFont()->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", GetTint());
    xmlInfo->SetString("Content", GetContent(), {XMLProperty::Inline});
    xmlInfo->SetFloat("TextSize", GetTextSize());
    xmlInfo->SetFloat("HSpacing", GetHorizontalSpacing());
    xmlInfo->SetBool("Kerning", GetKerning());
    xmlInfo->SetEnum("VerticalAlign", {"Top", "Center", "Bot"},
                     int(GetVerticalAlignment()), {});
    xmlInfo->SetEnum("HorizontalAlign", {"Left", "Center", "Right"},
                     int(GetHorizontalAlignment()), {});

    xmlInfo->GetAttribute("Tint")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
}
