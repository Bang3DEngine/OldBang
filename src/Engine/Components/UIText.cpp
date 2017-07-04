#include "Bang/UIText.h"

#include "Bang/Math.h"
#include "Bang/Font.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/G_Screen.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/G_GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/SelectionFramebuffer.h"
#endif

UIText::UIText() : UIRenderer()
{
    SetMesh( new Mesh() );
    SetMaterial(
         AssetsManager::Load<Material>( EPATH("Materials/UI/G_UIText.bmat") ));
    UseMaterialCopy();

    SetFont( AssetsManager::Load<Font>( EPATH("Fonts/UbuntuFont.bfont") ));
    SetContent("");

    SetRenderMode(GL::RenderMode::Quads);
}

UIText::~UIText()
{
    if (p_mesh) { delete p_mesh; }
}

Rect UIText::GetCharRect(char c) const
{
    if (!m_font) { return Rect::Empty; }

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

float UIText::GetCharAdvance(char current, char next) const
{
    if (!m_font) { return 0.0f; }

    int advancePx = -1;

    // TODO: Kerning
    // if (m_kerning && next != '\0')
    //{
    //    float kernX = m_font->GetKerningX(current, next);
    //    if (kernX > 5) { advancePx = kernX; }
    //}

    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(current);
    if (current == ' ' || charMetrics.width <= 0 ) { advancePx = 100; }
    if (advancePx < 0)
    {
        advancePx = charMetrics.advance;
    }
    return advancePx * GetTextSizeScaled().x;
}

void UIText::FillQuadsMeshPositions()
{
    if (!m_font) { p_mesh->LoadPositions({}); return; }

    List<Vector2> quadPos;

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
        contentBoundMin = Vector2::Min(contentBoundMin, charRectMin);
        contentBoundMax = Vector2::Max(contentBoundMax, charRectMax);

        quadPos.Add( Vector2(charRectMin.x, charRectMin.y) );
        quadPos.Add( Vector2(charRectMax.x, charRectMin.y) );
        quadPos.Add( Vector2(charRectMax.x, charRectMax.y) );
        quadPos.Add( Vector2(charRectMin.x, charRectMax.y) );

        const char nextChar = i < m_content.Length() - 1 ? m_content[i+1] : '\0';
        const float advance = GetCharAdvance(c, nextChar);
        totalAdv += advance + hSpacingNDC;
    }
    contentBoundMax.x = Math::Max(contentBoundMax.x,
                                  contentBoundMin.x + totalAdv);

    Array<Vector2> positions2d;
    Array<Vector3> positions3d;
    Vector2 contentSize = contentBoundMax - contentBoundMin;
    for (const Vector2 &pos : quadPos)
    {
        Vector2 posAligned = pos;
        ApplyAlignmentOffset(contentSize, &posAligned);
        positions3d.Add(Vector3(posAligned, 0));
        positions2d.Add(posAligned);
    }

    m_charQuadsRect = Rect::GetBoundingRectFromPositions(positions2d.To<List>());
    p_mesh->LoadPositions(positions3d);
}

void UIText::FillQuadsMeshUvs()
{
    if (!m_font) { p_mesh->LoadUvs({}); return; }

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

    p_mesh->LoadUvs(quadUvs);
}

void UIText::RefreshMesh()
{
    FillQuadsMeshPositions();
    FillQuadsMeshUvs();
    SetMesh(p_mesh);
}

void UIText::ApplyAlignmentOffset(const Vector2& contentSize,
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
        if (m_font)
        {
            GetMaterial()->SetTexture( m_font->GetAtlasTexture() );
        }
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

Rect UIText::GetNDCRect() const
{
    Matrix4 m; transform->GetLocalToWorldMatrix(&m);
    return m * m_charQuadsRect;
}

void UIText::Bind() const
{
    Rect screenNDCRect = GetNDCRect() * 1.1f;
    G_GBuffer *gbuffer = G_GraphicPipeline::GetActive()->GetG_GBuffer();
    gbuffer->PrepareColorReadBuffer(screenNDCRect);
    UIRenderer::Bind();
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

Vector2 UIText::GetTextSizeScaled() const
{
    constexpr double scaleFactor = 0.00015;
    return Vector2(m_textSize * scaleFactor);
}


void UIText::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);

    Path fontFilepath = xmlInfo.GetFilepath("Font");
    SetFont( AssetsManager::Load<Font>(fontFilepath) );

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
                             GetFont()->GetFilepath() : Path(), "bfont");
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
    // xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
}
