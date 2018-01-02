#include "Bang/UIImageRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/Image.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/RectTransform.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UIImageRenderer::UIImageRenderer()
{
    SetMaterial(MaterialFactory::GetUIImage().Get());

    p_quadMesh = Resources::Clone<Mesh>( MeshFactory::GetUIPlane() );
    SetHorizontalAlignment( GetHorizontalAlignment() );
    SetVerticalAlignment( GetVerticalAlignment() );
}

UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::OnRender()
{
    UIRenderer::OnRender();
    RegenerateQuadVAO();

    if (GetTint().a > 0.0f)
    {
        GL::Render(p_quadMesh.Get()->GetVAO(), GetRenderPrimitive(),
                   p_quadMesh.Get()->GetVertexCount());
    }
}

void UIImageRenderer::SetUvMultiply(const Vector2 &uvMultiply)
{
    if (uvMultiply != GetUvMultiply())
    {
        GetMaterial()->SetUvMultiply(uvMultiply);
        OnChanged();
    }
}

void UIImageRenderer::SetImageTexture(const Path &imagePath)
{
    RH<Texture2D> tex = Resources::Load<Texture2D>(imagePath);
    SetImageTexture(tex.Get());
}

void UIImageRenderer::SetImageTexture(Texture2D* imageTexture)
{
    if (imageTexture != GetImageTexture())
    {
        p_imageTexture.Set(imageTexture);
        GetMaterial()->SetTexture(p_imageTexture.Get());
        OnChanged();
    }
}

void UIImageRenderer::SetTint(const Color &tint)
{
    if (tint != GetTint())
    {
        GetMaterial()->SetDiffuseColor(tint);
        OnChanged();
    }
}

void UIImageRenderer::SetAspectRatioMode(AspectRatioMode arMode)
{
    if (arMode != GetAspectRatioMode())
    {
        m_aspectRatioMode = arMode;
        OnChanged();
    }
}

void UIImageRenderer::SetVerticalAlignment(VerticalAlignment verticalAlignment)
{
    if (verticalAlignment != GetVerticalAlignment())
    {
        m_verticalAlignment = verticalAlignment;
        OnChanged();
    }
}

void UIImageRenderer::SetHorizontalAlignment(HorizontalAlignment horizontalAlignment)
{
    if (horizontalAlignment != GetHorizontalAlignment())
    {
        m_horizontalAlignment = horizontalAlignment;
        OnChanged();
    }
}

const Vector2 &UIImageRenderer::GetUvMultiply() const
{
    return GetMaterial()->GetUvMultiply();
}

const Color &UIImageRenderer::GetTint() const
{
    return GetMaterial()->GetDiffuseColor();
}

Texture2D *UIImageRenderer::GetImageTexture() const
{
    return p_imageTexture.Get();
}

AspectRatioMode UIImageRenderer::GetAspectRatioMode() const
{
    return m_aspectRatioMode;
}

VerticalAlignment UIImageRenderer::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}

HorizontalAlignment UIImageRenderer::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

void UIImageRenderer::OnChanged()
{
    m_hasChanged = true;
    UIRenderer::PropagateOnUIRendererChanged();
}

void UIImageRenderer::RegenerateQuadVAO()
{
    if (!m_hasChanged) { return; }
    m_hasChanged = false;

    RectTransform *rt = GetGameObject()->GetRectTransform();
    Rect rectPx = rt->GetViewportRect();
    Vector2i rectSize(rectPx.GetSize());
    ENSURE(m_prevRectSize != rectSize);
    ENSURE(p_imageTexture);

    Vector2i texSize(p_imageTexture.Get()->GetSize());
    Vector2i texQuadSize =
            AspectRatio::GetAspectRatioedSize(rectSize, texSize,
                                              GetAspectRatioMode());

    Vector2 uvSize = Vector2(rectSize) / Vector2(texQuadSize);

    Vector2 uvMin = Vector2::Zero;
    Vector2 margMult = Vector2::Zero;
    const HorizontalAlignment hAlign = GetHorizontalAlignment();
    if      (hAlign == HorizontalAlignment::Center) { margMult.x = 0.5f; }
    else if (hAlign == HorizontalAlignment::Right)  { margMult.x = 1.0f; }

    const VerticalAlignment vAlign = GetVerticalAlignment();
    if      (vAlign == VerticalAlignment::Center) { margMult.y = 0.5f; }
    else if (vAlign == VerticalAlignment::Top)    { margMult.y = 1.0f; }

    uvMin += (1.0f-uvSize) * margMult;
    Vector2 uvMax = uvMin + uvSize;
    Array<Vector2> quadUvs = { Vector2(uvMin.x, uvMax.y),
                               Vector2(uvMax.x, uvMax.y),
                               Vector2(uvMax.x, uvMin.y),

                               Vector2(uvMin.x, uvMax.y),
                               Vector2(uvMax.x, uvMin.y),
                               Vector2(uvMin.x, uvMin.y)};

    p_quadMesh.Get()->LoadUvs(quadUvs);
}

void UIImageRenderer::OnTransformChanged()
{
    UIRenderer::OnTransformChanged();
    OnChanged();
}

Rect UIImageRenderer::GetBoundingRect(Camera *camera) const
{
    Rect boundingRect = UIRenderer::GetBoundingRect(camera);
    return Rect(boundingRect.GetCenter(), boundingRect.GetCenter());
}

void UIImageRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImageRenderer *img = Cast<UIImageRenderer*>(clone);
    img->SetImageTexture( GetImageTexture() );
    img->SetAspectRatioMode( GetAspectRatioMode() );
}

void UIImageRenderer::ImportXML(const XMLNode &xmlInfo)
{
    UIRenderer::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Image"))
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(xmlInfo.Get<GUID>("Image"));
        SetImageTexture(tex.Get());
    }

    if (xmlInfo.Contains("Tint"))
    { SetTint( xmlInfo.Get<Color>("Tint") ); }

    if (xmlInfo.Contains("HorizontalAlignment"))
    { SetHorizontalAlignment( xmlInfo.Get<HorizontalAlignment>("HorizontalAlignment") ); }

    if (xmlInfo.Contains("VerticalAlignment"))
    { SetVerticalAlignment( xmlInfo.Get<VerticalAlignment>("VerticalAlignment") ); }

    if (xmlInfo.Contains("AspectRatioMode"))
    { SetAspectRatioMode( xmlInfo.Get<AspectRatioMode>("AspectRatioMode") ); }
}

void UIImageRenderer::ExportXML(XMLNode *xmlInfo) const
{
    UIRenderer::ExportXML(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    xmlInfo->Set("Image", imgTex ? imgTex->GetGUID() : GUID::Empty());
    xmlInfo->Set("Tint", GetTint());
    xmlInfo->Set("HorizontalAlignment", GetHorizontalAlignment());
    xmlInfo->Set("VerticalAlignment", GetVerticalAlignment());
    xmlInfo->Set("AspectRatioMode", GetAspectRatioMode());
}
