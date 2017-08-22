#include "Bang/UITextRenderer.h"

#include "Bang/Math.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Screen.h"
#include "Bang/XMLNode.h"
#include "Bang/G_Screen.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/SceneManager.h"
#include "Bang/Resources.h"
#include "Bang/RectTransform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/TextFormatter.h"
#include "Bang/MaterialFactory.h"

UITextRenderer::UITextRenderer() : UIRenderer()
{
    m_mesh = new Mesh();
    SetMaterial( MaterialFactory::GetUIText() );
    UseMaterialCopy();

    SetFont( Resources::Load<Font>( EPATH("Fonts/UbuntuFont.bfont") ));
    SetContent("Bang");
    SetTextSize(20.0f);
    SetTextColor(Color::Black);

    SetRenderPrimitive(GL::Primitives::Quads);
    RefreshMesh();
}

UITextRenderer::~UITextRenderer()
{
    if (m_mesh) { delete m_mesh; }
}

void UITextRenderer::OnRender()
{
    Renderer::OnRender();
    GL::Render(m_mesh->GetVAO(), GetRenderPrimitive(), m_mesh->GetVertexCount());
}

void UITextRenderer::OnUpdate()
{
    UIRenderer::OnUpdate();
    if (m_hasChanged)
    {
        RefreshMesh();
    }
}

void UITextRenderer::OnParentSizeChanged()
{
    UIRenderer::OnParentSizeChanged();
    m_hasChanged = true;
}

void UITextRenderer::Bind() const
{
    // Nullify RectTransform model, since we control its position and size
    // directly from the VBO creation...
    transform->SetEnabled(false);
    UIRenderer::Bind();
}

void UITextRenderer::UnBind() const
{
    UIRenderer::UnBind();
    transform->SetEnabled(true);
}

void UITextRenderer::RefreshMesh()
{
    if (!m_hasChanged) { return; }
    m_hasChanged = false;

    if (!m_font)
    {
        m_mesh->LoadPositions({});
        m_mesh->LoadUvs({});
        return;
    }

    // Get the quad positions of the rects of each char
    RectTransform *rt = SCAST<RectTransform*>(transform); ENSURE(rt);
    Vector2i sizedSpacingPx = Vector2i(
                G_Font::ScaleMagnitude(Vector2f(GetSpacing()), GetTextSize()));
    Array<TextFormatter::CharRect> textCharRects =
            TextFormatter::GetFormattedTextPositions(
                                        GetContent(),
                                        GetFont(),
                                        GetHorizontalAlignment(),
                                        GetVerticalAlignment(),
                                        GetWrapping(),
                                        GetTextSize(),
                                        rt,
                                        sizedSpacingPx);

    // Generate quad positions and uvs for the mesh, and load them
    Array<Vector2> textQuadUvs;
    Array<Vector2> textQuadPos2D;
    Array<Vector3> textQuadPos3D;

    m_charRectsLocalNDC.Clear();
    for (const TextFormatter::CharRect &cr : textCharRects)
    {
        Vector2f minGlobalNDC = rt->FromLocalNDCToGlobalNDC(cr.rectLocalNDC.GetMin());
        Vector2f maxGlobalNDC = rt->FromLocalNDCToGlobalNDC(cr.rectLocalNDC.GetMax());
        Rect charRectGlobalNDC = Rect(minGlobalNDC, maxGlobalNDC);

        textQuadPos2D.PushBack(charRectGlobalNDC.GetMinXMinY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMinXMinY(), 0) );

        textQuadPos2D.PushBack(charRectGlobalNDC.GetMaxXMinY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMaxXMinY(), 0) );

        textQuadPos2D.PushBack(charRectGlobalNDC.GetMaxXMaxY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMaxXMaxY(), 0) );

        textQuadPos2D.PushBack(charRectGlobalNDC.GetMinXMaxY());
        textQuadPos3D.PushBack( Vector3(charRectGlobalNDC.GetMinXMaxY(), 0) );

        Vector2 minUv = m_font->GetCharMinUvInAtlas(cr.character);
        Vector2 maxUv = m_font->GetCharMaxUvInAtlas(cr.character);
        textQuadUvs.PushBack( Vector2(minUv.x, maxUv.y) );
        textQuadUvs.PushBack( Vector2(maxUv.x, maxUv.y) );
        textQuadUvs.PushBack( Vector2(maxUv.x, minUv.y) );
        textQuadUvs.PushBack( Vector2(minUv.x, minUv.y) );

        m_charRectsLocalNDC.PushBack(cr.rectLocalNDC);
    }

    m_textRectNDC = Rect::GetBoundingRectFromPositions(textQuadPos2D.Begin(),
                                                       textQuadPos2D.End());
    m_mesh->LoadPositions(textQuadPos3D);
    m_mesh->LoadUvs(textQuadUvs);
}

void UITextRenderer::SetHorizontalAlign(HorizontalAlignment horizontalAlignment)
{
    if (GetHorizontalAlignment() != horizontalAlignment)
    {
        m_horizontalAlignment = horizontalAlignment;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetVerticalAlign(VerticalAlignment verticalAlignment)
{
    if (GetVerticalAlignment() != verticalAlignment)
    {
        m_verticalAlignment = verticalAlignment;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetFont(Font *font)
{
    if (GetFont() != font)
    {
        m_font = font;
        if (m_font)
        {
            // TODO MEMLEAK
            Texture2D *tex = new Texture2D(m_font->GetAtlasTexture());
            GetMaterial()->SetTexture(tex);
        }
        m_hasChanged = true;
    }
}

void UITextRenderer::SetKerning(bool kerning)
{
    if (GetKerning() != kerning)
    {
        m_kerning = kerning;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetWrapping(bool wrapping)
{
    if (GetWrapping() != wrapping)
    {
        m_wrapping = wrapping;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetContent(const String &content)
{
    if (GetContent() != content)
    {
        m_content = content;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetTextSize(int size)
{
    if (GetTextSize() != size)
    {
        m_textSize = size;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetSpacing(const Vector2i& spacing)
{
    if (GetSpacing() != spacing)
    {
        m_spacing = spacing;
        m_hasChanged = true;
    }
}

void UITextRenderer::SetTextColor(const Color &textColor)
{
    GetMaterial()->SetDiffuseColor( textColor );
}

Font *UITextRenderer::GetFont() const { return m_font; }
bool UITextRenderer::GetKerning() const { return m_kerning; }
bool UITextRenderer::GetWrapping() const { return m_wrapping; }
const String &UITextRenderer::GetContent() const { return m_content; }
int UITextRenderer::GetTextSize() const { return m_textSize; }
Vector2i UITextRenderer::GetSpacing() const { return m_spacing; }
const Array<Rect> &UITextRenderer::GetCharRectsLocalNDC() const
{
    return m_charRectsLocalNDC;
}
const Rect &UITextRenderer::GetCharRectLocalNDC(uint charIndex) const
{
    return m_charRectsLocalNDC[charIndex];
}

Rect UITextRenderer::GetContentGlobalNDCRect() const { return m_textRectNDC; }
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
    return GetContentGlobalNDCRect();
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
    text->SetSpacing( GetSpacing() );
    text->SetWrapping( GetWrapping() );
    text->SetHorizontalAlign( GetHorizontalAlignment() );
    text->SetVerticalAlign( GetVerticalAlignment() );
}

void UITextRenderer::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);

    SetFont( Resources::Load<Font>( xmlInfo.Get<GUID>("Font") ) );
    SetContent(xmlInfo.Get<String>("Content"));
    SetTextSize(xmlInfo.Get<float>("TextSize"));
    SetSpacing(xmlInfo.Get<Vector2i>("Spacing"));
    SetKerning(xmlInfo.Get<bool>("Kerning"));
    SetTextColor( xmlInfo.Get<Color>("TextColor") );
    SetWrapping( xmlInfo.Get<bool>("Wrapping") );
    SetVerticalAlign( xmlInfo.Get<VerticalAlignment>("VerticalAlign") );
    SetHorizontalAlign( xmlInfo.Get<HorizontalAlignment>("HorizontalAlign"));
}

void UITextRenderer::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    xmlInfo->Set("Font", GetFont() ? GetFont()->GetGUID() : GUID::Empty());
    xmlInfo->Set("Content", GetContent());
    xmlInfo->Set("TextSize", GetTextSize());
    xmlInfo->Set("Spacing", GetSpacing());
    xmlInfo->Set("TextColor", GetTextColor());
    xmlInfo->Set("Kerning", GetKerning());
    xmlInfo->Set("Wrapping", GetWrapping());
    xmlInfo->Set("VerticalAlign", GetVerticalAlignment() );
    xmlInfo->Set("HorizontalAlign", GetHorizontalAlignment() );
}
