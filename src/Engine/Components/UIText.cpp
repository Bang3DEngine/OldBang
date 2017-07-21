#include "Bang/UIText.h"

#include "Bang/Math.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/G_Screen.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"

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

    const Vector2 textSize = GetTextSizeScaled();
    Font::CharGlyphMetrics charMetrics = m_font->GetCharacterMetrics(c);

    const Vector2 charSize =
            Vector2(charMetrics.width, charMetrics.height) * textSize;
    const Vector2 bearing =
            Vector2(charMetrics.bearingX, charMetrics.bearingY) * textSize;

    Vector2 charMin(bearing.x, bearing.y - charSize.y);
    Vector2 charMax(bearing.x + charSize.x, bearing.y);
    return Rect(charMin, charMax);
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
    RectTransform *rt = SCAST<RectTransform*>(transform); ENSURE(rt);

    List<Vector2> quadPos;
    const Rect ndcRect       = rt->GetScreenSpaceRect();
    Vector2 totalAdvance     = Vector2(ndcRect.GetMin().x, ndcRect.GetMax().y);
    const Vector2 spacingNDC = m_spacing * GetTextSizeScaled();
    for (int i = 0; i < m_content.Length(); ++i)
    {
        const char c = m_content[i];

        Rect charRect = GetCharRect(c);
        if (charRect.GetMax().x + totalAdvance.x >= ndcRect.GetMax().x)
        {
            totalAdvance.x  = ndcRect.GetMin().x;
            totalAdvance.y -= spacingNDC.y;
        }
        charRect += totalAdvance;

        quadPos.Add( charRect.GetBotLeft()  );
        quadPos.Add( charRect.GetBotRight() );
        quadPos.Add( charRect.GetTopRight() );
        quadPos.Add( charRect.GetTopLeft()  );

        const char nextChar = i < m_content.Length() - 1 ? m_content[i + 1] : '\0';
        totalAdvance.x += GetCharAdvance(c, nextChar) + spacingNDC.x;
    }

    m_textRectNDC = Rect::GetBoundingRectFromPositions(quadPos);
    const Vector2 alOffset = GetAlignmentOffset(m_textRectNDC);

    List<Vector2> positions2d;
    List<Vector3> positions3d;
    for (const Vector2 &pos : quadPos)
    {
        positions3d.Add(Vector3(pos + alOffset, 0));
        positions2d.Add(pos + alOffset);
    }
    m_textRectNDC = Rect::GetBoundingRectFromPositions(positions2d);
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

Vector2 UIText::GetAlignmentOffset(const Rect &contentRect) const
{
    RectTransform *rt = SCAST<RectTransform*>(transform);
    if (!rt) { return Vector2::Zero; }

    Vector2 offset = Vector2::Zero;
    Rect rtRectNDC = rt->GetScreenSpaceRect();
    if (m_horizontalAlignment == HorizontalAlignment::Left)
    {
        offset.x = rtRectNDC.GetMin().x - contentRect.GetMin().x;
    }
    if (m_horizontalAlignment == HorizontalAlignment::Center)
    {
        offset.x = rtRectNDC.GetCenter().x - contentRect.GetCenter().x;
    }
    else if (m_horizontalAlignment == HorizontalAlignment::Right)
    {
        offset.x = rtRectNDC.GetMax().x - contentRect.GetMax().x;
    }

    if (m_verticalAlignment == VerticalAlignment::Top)
    {
        offset.y = rtRectNDC.GetMin().y - contentRect.GetMin().y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Center)
    {
        offset.y = rtRectNDC.GetCenter().y - contentRect.GetCenter().y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Bot)
    {
        offset.y = rtRectNDC.GetMax().y - contentRect.GetMax().y;
    }

    return offset;
}

void UIText::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);

    UIText *text = SCAST<UIText*>(clone);
    text->SetMesh( new Mesh() );
    text->SetFont ( GetFont() );
    text->SetContent( GetContent() );
    text->SetTextSize( GetTextSize() );
    text->SetSpacing( GetSpacing() );
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
            // TODO MEMLEAK
            Texture2D *tex = new Texture2D(m_font->GetAtlasTexture());
            GetMaterial()->SetTexture(tex);
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

void UIText::SetSpacing(const Vector2& spacing)
{
    if (m_spacing != spacing)
    {
        m_spacing = spacing;
        RefreshMesh();
    }
}

Vector2 UIText::GetSpacing() const
{
    return m_spacing;
}

Rect UIText::GetNDCRect() const
{
    return m_textRectNDC;
}

void UIText::Bind() const
{
    Rect screenNDCRect = GetNDCRect() * 1.1f;
    G_GBuffer *gbuffer = GraphicPipeline::GetActive()->GetGBuffer();
    gbuffer->PrepareColorReadBuffer(screenNDCRect);

    // Nullify RectTransform model, since we control its position and size
    // directly from the VBO creation...
    transform->SetEnabled(false);
    UIRenderer::Bind();
}

void UIText::UnBind() const
{
    UIRenderer::UnBind();
    transform->SetEnabled(true);
}

void UIText::OnParentSizeChanged()
{
    UIRenderer::OnParentSizeChanged();
    RefreshMesh();
}

void UIText::OnDrawGizmos(GizmosPassType gizmosPassType)
{
    UIRenderer::OnDrawGizmos(gizmosPassType);

    Gizmos::RenderRect(m_textRectNDC);
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
    SetSpacing(xmlInfo.GetVector2("Spacing"));
    SetKerning(xmlInfo.GetBool("Kerning"));

    SetVerticalAlign( xmlInfo.GetEnum<VerticalAlignment>("VerticalAlign") );
    SetHorizontalAlign( xmlInfo.GetEnum<HorizontalAlignment>("HorizontalAlign"));

    RefreshMesh();
}

void UIText::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    xmlInfo->SetFilepath("Font", GetFont() ? GetFont()->GetFilepath() :
                                             Path::Empty);
    xmlInfo->SetColor("Color", GetTint());
    xmlInfo->SetString("Content", GetContent());
    xmlInfo->SetFloat("TextSize", GetTextSize());
    xmlInfo->SetVector2("Spacing", GetSpacing());
    xmlInfo->SetBool("Kerning", GetKerning());
    xmlInfo->SetEnum<VerticalAlignment>("VerticalAlign",
                                        GetVerticalAlignment() );
    xmlInfo->SetEnum<HorizontalAlignment>("HorizontalAlign",
                                          GetHorizontalAlignment() );
}
