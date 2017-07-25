#include "Bang/UIText.h"

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
#include "Bang/AssetsManager.h"
#include "Bang/RectTransform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/TextFormatter.h"

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

void UIText::RefreshMesh()
{
    if (!m_font)
    {
        p_mesh->LoadPositions({});
        p_mesh->LoadUvs({});
        return;
    }

    // Get the quad positions of the rects of each char
    RectTransform *rt = SCAST<RectTransform*>(transform); ENSURE(rt);
    Array<TextFormatter::CharRect> textCharRects =
            TextFormatter::GetFormattedTextPositions(
                                        GetContent(),
                                        GetFont(),
                                        GetHorizontalAlignment(),
                                        GetVerticalAlignment(),
                                        GetHorizontalWrapMode(),
                                        GetVerticalWrapMode(),
                                        GetTextSize(),
                                        GetSpacing(),
                                        rt->GetScreenSpaceRect());

    // Generate quad positions and uvs for the mesh, and load them
    Array<Vector2> textQuadUvs;
    Array<Vector2> textQuadPositions2D;
    Array<Vector3> textQuadPositions3D;
    for (const TextFormatter::CharRect &cr : textCharRects)
    {
        Vector2 minUv = m_font->GetCharMinUvInAtlas(cr.character);
        Vector2 maxUv = m_font->GetCharMaxUvInAtlas(cr.character);

        textQuadPositions2D.PushBack(cr.rect.GetMinXMinY());
        textQuadPositions3D.PushBack( Vector3(cr.rect.GetMinXMinY(), 0) );
        textQuadUvs.PushBack( Vector2(minUv.x, maxUv.y) );

        textQuadPositions2D.PushBack(cr.rect.GetMaxXMinY());
        textQuadPositions3D.PushBack( Vector3(cr.rect.GetMaxXMinY(), 0) );
        textQuadUvs.PushBack( Vector2(maxUv.x, maxUv.y) );

        textQuadPositions2D.PushBack(cr.rect.GetMaxXMaxY());
        textQuadPositions3D.PushBack( Vector3(cr.rect.GetMaxXMaxY(), 0) );
        textQuadUvs.PushBack( Vector2(maxUv.x, minUv.y) );

        textQuadPositions2D.PushBack(cr.rect.GetMinXMaxY());
        textQuadPositions3D.PushBack( Vector3(cr.rect.GetMinXMaxY(), 0) );
        textQuadUvs.PushBack( Vector2(minUv.x, minUv.y) );
    }

    m_textRectNDC = Rect::GetBoundingRectFromPositions(textQuadPositions2D);
    p_mesh->LoadPositions(textQuadPositions3D);
    p_mesh->LoadUvs(textQuadUvs);
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
        offset.y = rtRectNDC.GetMax().y - contentRect.GetMax().y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Center)
    {
        offset.y = rtRectNDC.GetCenter().y - contentRect.GetCenter().y;
    }
    else if (m_verticalAlignment == VerticalAlignment::Bot)
    {
        offset.y = rtRectNDC.GetMin().y - contentRect.GetMin().y;
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
    text->SetHorizontalWrapMode( GetHorizontalWrapMode() );
    text->SetVerticalWrapMode( GetVerticalWrapMode() );
    text->SetHorizontalAlign( GetHorizontalAlignment() );
    text->SetVerticalAlign( GetVerticalAlignment() );
    text->RefreshMesh();
}

void UIText::SetHorizontalAlign(HorizontalAlignment horizontalAlignment)
{
    if (m_horizontalAlignment != horizontalAlignment)
    {
        m_horizontalAlignment = horizontalAlignment;
        RefreshMesh();
    }
}

void UIText::SetVerticalAlign(VerticalAlignment verticalAlignment)
{
    if (m_verticalAlignment != verticalAlignment)
    {
        m_verticalAlignment = verticalAlignment;
        RefreshMesh();
    }
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

void UIText::SetKerning(bool kerning)
{
    if (m_kerning != kerning)
    {
        m_kerning = kerning;
        RefreshMesh();
    }
}

void UIText::SetHorizontalWrapMode(WrapMode wrapMode)
{
    if (m_hWrapMode != wrapMode)
    {
        m_hWrapMode = wrapMode;
        RefreshMesh();
    }
}

void UIText::SetVerticalWrapMode(WrapMode wrapMode)
{
    if (m_vWrapMode != wrapMode)
    {
        m_vWrapMode = wrapMode;
        RefreshMesh();
    }
}


void UIText::SetContent(const String &content)
{
    if (m_content != content)
    {
        m_content = content;
        RefreshMesh();
    }
}

void UIText::SetTextSize(int size)
{
    if (m_textSize != size)
    {
        m_textSize = size;
        RefreshMesh();
    }
}

void UIText::SetSpacing(const Vector2& spacing)
{
    if (m_spacing != spacing)
    {
        m_spacing = spacing;
        RefreshMesh();
    }
}

Font *UIText::GetFont() const { return m_font; }
bool UIText::GetKerning() const { return m_kerning; }
WrapMode UIText::GetVerticalWrapMode() const { return m_vWrapMode; }
WrapMode UIText::GetHorizontalWrapMode() const { return m_hWrapMode; }
const String &UIText::GetContent() const { return m_content; }
int UIText::GetTextSize() const { return m_textSize; }
Vector2 UIText::GetSpacing() const { return m_spacing; }
Rect UIText::GetNDCRect() const { return m_textRectNDC; }
VerticalAlignment UIText::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}
HorizontalAlignment UIText::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

void UIText::Bind() const
{
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

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return GetNDCRect();
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

    SetVerticalWrapMode( xmlInfo.GetEnum<WrapMode>("VWrapMode") );
    SetHorizontalWrapMode( xmlInfo.GetEnum<WrapMode>("HWrapMode") );
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
    xmlInfo->SetEnum<WrapMode>("VWrapMode", GetVerticalWrapMode());
    xmlInfo->SetEnum<WrapMode>("HWrapMode", GetHorizontalWrapMode());
    xmlInfo->SetEnum<VerticalAlignment>("VerticalAlign",
                                        GetVerticalAlignment() );
    xmlInfo->SetEnum<HorizontalAlignment>("HorizontalAlign",
                                          GetHorizontalAlignment() );
}
