#include "Bang/UITextRenderer.h"

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/TextFormatter.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UITextRenderer::UITextRenderer() : UIRenderer()
{
    m_mesh = new Mesh();

    SetMaterial( MaterialFactory::GetUIText() );
    UseMaterialCopy();

    SetFont( Resources::Load<Font>( EPATH("Fonts/Ubuntu.ttf") ));
    SetContent("Bang");
    SetTextSize(20.0f);
    SetTextColor(Color::Black);

    SetRenderPrimitive(GL::Primitives::Triangles);
    OnChanged();
}

UITextRenderer::~UITextRenderer()
{
    if (m_mesh) { delete m_mesh; }
}

void UITextRenderer::OnRender()
{
    UIRenderer::OnRender();

    int vertCount = (IsOverlapping() ? 4 : m_mesh->GetVertexCount());
    int startVert = (IsOverlapping() ? m_currentRenderingChar * 4 : 0);
    GL::Render(m_mesh->GetVAO(), GetRenderPrimitive(), vertCount, startVert);
}

void UITextRenderer::OnRender(RenderPass renderPass)
{
    if (m_hasToRegenerateVAO) { RegenerateCharQuadsVAO(); }
    if (!IsOverlapping())
    {
        // Render all quads at the same time
        m_currentRenderingChar = 0;
        UIRenderer::OnRender(renderPass);
    }
    else
    {
        // Render character by character
        for (int i = 0; i < GetContent().Size(); ++i)
        {
            m_currentRenderingChar = i;
            UIRenderer::OnRender(renderPass);
        }
    }
}

Vector2i UITextRenderer::_GetMinSize() const
{
    switch (GetLayoutMode())
    {
        case LayoutMode::SingleLineMinPreferred:
        case LayoutMode::MultiLineMinPreferred:
            return _GetPreferredSize();

        default: return Vector2i::Zero;
    };
}

Vector2i UITextRenderer::_GetPreferredSize() const
{
    if (!GetFont()) { return Vector2i::Zero; }

    GetFont()->SetMetricsSize( GetTextSize() );

    Vector2i prefSize;
    switch (GetLayoutMode())
    {
        case LayoutMode::SingleLinePreferred:
        case LayoutMode::SingleLineMinPreferred:
            prefSize = TextFormatter::GetTextSizeOneLined(GetContent(),
                                                          GetFont(),
                                                          GetSpacingMultiplier());
        break;

        case LayoutMode::MultiLinePreferred:
        case LayoutMode::MultiLineMinPreferred:
        {
            const Array<Rect>& crs = GetCharRectsLocalNDC();
            Rect boundingRect = Rect::Union(crs.Begin(), crs.End());
            Recti boundingRectPx = GL::FromGlobalNDCToPixels(boundingRect);
            prefSize = boundingRectPx.GetSize();
        }
        break;
    }

    return prefSize;
}

Vector2 UITextRenderer::_GetFlexibleSize() const
{
    return Vector2(0);
}

void UITextRenderer::RegenerateCharQuadsVAO()
{
    m_hasToRegenerateVAO = false;
    ENSURE(gameObject);

    if (!m_font)
    {
        m_mesh->LoadPositions({});
        m_mesh->LoadUvs({});
        return;
    }

    // Get the quad positions of the rects of each char
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);

    m_font->SetMetricsSize( GetTextSize() );
    Array<TextFormatter::CharRect> textCharRects =
            TextFormatter::GetFormattedTextPositions(
                                        GetContent(),
                                        GetFont(),
                                        rt->GetScreenSpaceRectPx(),
                                        GetSpacingMultiplier(),
                                        GetHorizontalAlignment(),
                                        GetVerticalAlignment(),
                                        IsWrapping());

    // Generate quad positions and uvs for the mesh, and load them
    Array<Vector2> textQuadUvs;
    Array<Vector2> textQuadPos2D;
    Array<Vector3> textQuadPos3D;

    m_charRectsLocalNDC.Clear();
    for (const TextFormatter::CharRect &cr : textCharRects)
    {
        if (!GetFont()->HasCharacter(cr.character)) { continue; }

        Vector2 minPxPerf = cr.rectPx.GetMin(); // Vector2::Round( cr.rectPx.GetMin() ) + 0.5f;
        Vector2 maxPxPerf = cr.rectPx.GetMax(); // Vector2::Round( cr.rectPx.GetMax() ) + 0.5f;
        Vector2f minGlobalNDC ( GL::FromPixelsPointToGlobalNDC(minPxPerf) );
        Vector2f maxGlobalNDC ( GL::FromPixelsPointToGlobalNDC(maxPxPerf) );

        Vector2 minUv = m_font->GetCharMinUvInAtlas(cr.character);
        Vector2 maxUv = m_font->GetCharMaxUvInAtlas(cr.character);

        if (m_font->IsUsingDistanceField())
        {
            // Scale the character quad and uvs so that the character is as
            // large as if we weren't using SDF. If we dont compensate
            // this size, we would get all the distance field in the same quad,
            // and consequently the character itself would be smaller
            Vector2 spOffsetPx = Vector2(m_font->GetSDFSpreadOffsetPx(cr.character));
            Vector2 spOffsetUv( Vector2(spOffsetPx) /
                                Vector2(m_font->GetAtlasTexture()->GetSize()) );

            Vector2 uvSize = (maxUv-minUv);
            Vector2 uvScaling = (uvSize + spOffsetUv * 2.0f) / uvSize;
            Vector2 globalNDCCharSize  = (maxGlobalNDC - minGlobalNDC);
            Vector2 globalNDCPosCenter = (maxGlobalNDC + minGlobalNDC) / 2.0f;
            Vector2 scaledSize = (globalNDCCharSize * uvScaling);
            minGlobalNDC = globalNDCPosCenter - scaledSize * 0.5f;
            maxGlobalNDC = globalNDCPosCenter + scaledSize * 0.5f;
            minUv -= spOffsetUv;
            maxUv += spOffsetUv;
        }

        Rect charRectGlobalNDC(minGlobalNDC, maxGlobalNDC);

        textQuadUvs.PushBack( Vector2(minUv.x, maxUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMinXMinY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMinXMinY(), 0) );
        textQuadUvs.PushBack( Vector2(maxUv.x, maxUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMaxXMinY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMaxXMinY(), 0) );
        textQuadUvs.PushBack( Vector2(maxUv.x, minUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMaxXMaxY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMaxXMaxY(), 0) );

        textQuadUvs.PushBack( Vector2(minUv.x, maxUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMinXMinY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMinXMinY(), 0) );
        textQuadUvs.PushBack( Vector2(maxUv.x, minUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMaxXMaxY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMaxXMaxY(), 0) );
        textQuadUvs.PushBack( Vector2(minUv.x, minUv.y) );
        textQuadPos2D.PushBack(charRectGlobalNDC.GetMinXMaxY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMinXMaxY(), 0) );

        Rect charRectLocalNDC (
                    rt->FromPixelsPointToLocalNDC( cr.rectPx.GetMin() ),
                    rt->FromPixelsPointToLocalNDC( cr.rectPx.GetMax() ) );
        m_charRectsLocalNDC.PushBack(charRectLocalNDC);
    }

    m_textRectNDC = Rect::GetBoundingRectFromPositions(textQuadPos2D.Begin(),
                                                       textQuadPos2D.End());
    m_mesh->LoadPositions(textQuadPos3D);
    m_mesh->LoadUvs(textQuadUvs);
}

void UITextRenderer::Bind() const
{
    // Nullify RectTransform model, since we control its position and size
    // directly from the VBO creation...
    Vector3 translate(0, 0, gameObject->transform->GetPosition().z);
    GL::SetModelMatrix( Matrix4::TranslateMatrix(translate) );
    gameObject->transform->SetEnabled(false);
    UIRenderer::Bind();

    if (GetFont())
    {
        const int textSize = Math::Max(GetTextSize(), 1);

        Vector2 atlasSize = Vector2(GetFont()->GetAtlasTexture()->GetSize());
        GL::Uniform("B_fontAtlasSize", atlasSize, false);

        bool usingDistField = GetFont()->IsUsingDistanceField();
        GL::Uniform("B_usingDistField", usingDistField,  false);
        if (usingDistField)
        {
            GL::Uniform("B_outlineWidth", GetOutlineWidth(), false);
            if (GetOutlineWidth() > 0.0f)
            {
                GL::Uniform("B_outlineColor", GetOutlineColor(), false);
                GL::Uniform("B_outlineBlurriness", GetOutlineBlurriness(), false);
            }

            float blurriness = GetBlurriness() / textSize;
            blurriness = Math::Clamp(blurriness, 0.0f, 1.0f);
            GL::Uniform("B_textBlurriness", blurriness, false);

            float alphaThresh = GetAlphaThreshold() + (0.05f / textSize);
            alphaThresh = Math::Clamp(alphaThresh, 0.0f, 1.0f);
            GL::Uniform("B_textAlphaThreshold", alphaThresh, false);
        }
    }
}

void UITextRenderer::UnBind() const
{
    UIRenderer::UnBind();
    gameObject->transform->SetEnabled(true);
}

void UITextRenderer::SetHorizontalAlign(HorizontalAlignment horizontalAlignment)
{
    if (GetHorizontalAlignment() != horizontalAlignment)
    {
        m_horizontalAlignment = horizontalAlignment;
        OnChanged();
    }
}

void UITextRenderer::SetVerticalAlign(VerticalAlignment verticalAlignment)
{
    if (GetVerticalAlignment() != verticalAlignment)
    {
        m_verticalAlignment = verticalAlignment;
        OnChanged();
    }
}

void UITextRenderer::SetFont(Font *font)
{
    if (GetFont() != font)
    {
        m_font = font;
        if (m_font)
        {
            GetMaterial()->SetTexture(m_font->GetAtlasTexture());
        }
        OnChanged();
    }
}

void UITextRenderer::SetKerning(bool kerning)
{
    if (IsKerning() != kerning)
    {
        m_kerning = kerning;
        OnChanged();
    }
}

void UITextRenderer::SetWrapping(bool wrapping)
{
    if (IsWrapping() != wrapping)
    {
        m_wrapping = wrapping;
        OnChanged();
    }
}

void UITextRenderer::SetOverlapping(bool overlapping)
{
    m_isOverlapping = overlapping;
}

void UITextRenderer::SetAlphaThreshold(float alphaThreshold)
{
    m_alphaThreshold = alphaThreshold;
}

void UITextRenderer::SetBlurriness(float blurriness)
{
    m_blurriness = blurriness;
}

void UITextRenderer::SetContent(const String &content)
{
    if (GetContent() != content)
    {
        m_content = content;
        OnChanged();
    }
}

void UITextRenderer::SetTextSize(int size)
{
    if (GetTextSize() != size)
    {
        m_textSize = Math::Max(size, 1);
        OnChanged();
    }
}

void UITextRenderer::SetOutlineWidth(float outlineWidth)
{
    m_outlineWidth = outlineWidth;
}

void UITextRenderer::SetOutlineColor(const Color &color)
{
    m_outlineColor = color;
}

void UITextRenderer::SetOutlineBlurriness(float outlineBlurriness)
{
    m_outlineBlurriness = outlineBlurriness;
}

void UITextRenderer::SetSpacingMultiplier(const Vector2& spacingMultiplier)
{
    if (GetSpacingMultiplier() != spacingMultiplier)
    {
        m_spacingMultiplier = spacingMultiplier;
        OnChanged();
    }
}

void UITextRenderer::SetLayoutMode(UITextRenderer::LayoutMode layoutMode)
{
    if (GetLayoutMode() != layoutMode)
    {
        m_layoutMode = layoutMode;
        OnChanged();
    }
}

bool UITextRenderer::NeedsReadingColorBuffer() const { return true; }

void UITextRenderer::SetTextColor(const Color &textColor)
{
    GetMaterial()->SetDiffuseColor( textColor );
}

Font *UITextRenderer::GetFont() const { return m_font; }
bool UITextRenderer::IsKerning() const { return m_kerning; }
bool UITextRenderer::IsWrapping() const { return m_wrapping; }

float UITextRenderer::GetBlurriness() const { return m_blurriness; }
float UITextRenderer::GetAlphaThreshold() const { return m_alphaThreshold; }
const String &UITextRenderer::GetContent() const { return m_content; }
int UITextRenderer::GetTextSize() const { return m_textSize; }

bool UITextRenderer::IsOverlapping() const { return m_isOverlapping; }
float UITextRenderer::GetOutlineWidth() const { return m_outlineWidth; }
const Color &UITextRenderer::GetOutlineColor() const { return m_outlineColor; }

float UITextRenderer::GetOutlineBlurriness() const { return m_outlineBlurriness; }
const Vector2& UITextRenderer::GetSpacingMultiplier() const { return m_spacingMultiplier; }
const Array<Rect> &UITextRenderer::GetCharRectsLocalNDC() const
{
    return m_charRectsLocalNDC;
}
const Rect &UITextRenderer::GetCharRectLocalNDC(uint charIndex) const
{
    return m_charRectsLocalNDC[charIndex];
}

Rect UITextRenderer::GetContentGlobalNDCRect() const { return m_textRectNDC; }

UITextRenderer::LayoutMode UITextRenderer::GetLayoutMode() const
{
    return m_layoutMode;
}
VerticalAlignment UITextRenderer::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}
HorizontalAlignment UITextRenderer::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

Rect UITextRenderer::GetBoundingRect(Camera *camera) const
{
    if (!IsOverlapping()) { return GetContentGlobalNDCRect(); }
    else
    {
        Vector2 p1 = m_mesh->GetPositions()[m_currentRenderingChar * 6 + 0].xy();
        Vector2 p2 = m_mesh->GetPositions()[m_currentRenderingChar * 6 + 3].xy();
        return Rect(Vector2::Min(p1,p2), Vector2::Max(p1,p2));
    }
}

void UITextRenderer::OnRectTransformChanged()
{
    OnChanged();
}

const Color &UITextRenderer::GetTextColor() const
{
    return GetMaterial()->GetDiffuseColor();
}

void UITextRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);

    UITextRenderer *text = SCAST<UITextRenderer*>(clone);
    text->SetFont ( GetFont() );
    text->SetContent( GetContent() );
    text->SetTextSize( GetTextSize() );
    text->SetTextColor( GetTextColor() );
    text->SetBlurriness( GetBlurriness() );
    text->SetAlphaThreshold( GetAlphaThreshold() );
    text->SetOverlapping( IsOverlapping() );
    text->SetOutlineWidth( GetOutlineWidth() );
    text->SetOutlineColor( GetOutlineColor() );
    text->SetOutlineBlurriness( GetOutlineBlurriness() );
    text->SetSpacingMultiplier( GetSpacingMultiplier() );
    text->SetWrapping( IsWrapping() );
    text->SetHorizontalAlign( GetHorizontalAlignment() );
    text->SetVerticalAlign( GetVerticalAlignment() );
}

void UITextRenderer::ImportXML(const XMLNode &xml)
{
    UIRenderer::ImportXML(xml);

    if (xml.Contains("Font"))
    { SetFont( Resources::Load<Font>( xml.Get<GUID>("Font") ) ); }

    if (xml.Contains("Content"))
    { SetContent(xml.Get<String>("Content")); }

    if (xml.Contains("TextSize"))
    { SetTextSize(xml.Get<float>("TextSize")); }

    if (xml.Contains("SpacingMultiplier"))
    { SetSpacingMultiplier(xml.Get<Vector2>("SpacingMultiplier")); }

    if (xml.Contains("Kerning"))
    { SetKerning(xml.Get<bool>("Kerning")); }

    if (xml.Contains("TextColor"))
    { SetTextColor( xml.Get<Color>("TextColor") ); }

    if (xml.Contains("AlphaThreshold"))
    { SetWrapping( xml.Get<bool>("AlphaThreshold") ); }

    if (xml.Contains("Wrapping"))
    { SetWrapping( xml.Get<bool>("Wrapping") ); }

    if (xml.Contains("VerticalAlign"))
    { SetVerticalAlign( xml.Get<VerticalAlignment>("VerticalAlign") ); }

    if (xml.Contains("HorizontalAlign"))
    { SetHorizontalAlign( xml.Get<HorizontalAlignment>("HorizontalAlign")); }
}

void UITextRenderer::ExportXML(XMLNode *xmlInfo) const
{
    UIRenderer::ExportXML(xmlInfo);

    xmlInfo->Set("Font", GetFont() ? GetFont()->GetGUID() : GUID::Empty());
    xmlInfo->Set("Content", GetContent());
    xmlInfo->Set("TextSize", GetTextSize());
    xmlInfo->Set("SpacingMultiplier", GetSpacingMultiplier());
    xmlInfo->Set("TextColor", GetTextColor());
    xmlInfo->Set("AlphaThreshold", GetAlphaThreshold());
    xmlInfo->Set("Kerning", IsKerning());
    xmlInfo->Set("Wrapping", IsWrapping());
    xmlInfo->Set("VerticalAlign", GetVerticalAlignment() );
    xmlInfo->Set("HorizontalAlign", GetHorizontalAlignment() );
}

void UITextRenderer::OnChanged()
{
    m_hasToRegenerateVAO = true;
    Invalidate();
}
