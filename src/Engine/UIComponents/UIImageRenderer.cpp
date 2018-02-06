#include "Bang/UIImageRenderer.h"

#include "Bang/GL.h"
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
}

UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::OnRender()
{
    UIRenderer::OnRender();

    if (GetTint().a > 0.0f)
    {
        GL::Render(p_quadMesh.Get()->GetVAO(), GetRenderPrimitive(),
                   p_quadMesh.Get()->GetVertexCount());
    }
}

void UIImageRenderer::SetImageTexture(const Path &imagePath)
{
    if (imagePath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(imagePath);
        SetImageTexture(tex.Get());
    }
    else { SetImageTexture(nullptr); }
}

void UIImageRenderer::SetImageTexture(Texture2D* imageTexture)
{
    if (imageTexture != GetImageTexture())
    {
        p_imageTexture.Set(imageTexture);
        GetMaterial()->SetTexture( GetImageTexture() );
    }
}

void UIImageRenderer::SetTint(const Color &tint)
{
    if (tint != GetTint())
    {
        m_tint = tint;
        GetMaterial()->SetDiffuseColor( GetTint() );
    }
}

const Color &UIImageRenderer::GetTint() const
{
    return m_tint;
}

Texture2D *UIImageRenderer::GetImageTexture() const
{
    return p_imageTexture.Get();
}

void UIImageRenderer::OnTransformChanged()
{
    UIRenderer::OnTransformChanged();
}

AARect UIImageRenderer::GetBoundingRect(Camera *camera) const
{
    AARect boundingRect = UIRenderer::GetBoundingRect(camera);
    return AARect(boundingRect.GetCenter(), boundingRect.GetCenter());
}

void UIImageRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImageRenderer *img = Cast<UIImageRenderer*>(clone);
    img->SetImageTexture( GetImageTexture() );
    img->SetTint( GetTint() );
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
}

void UIImageRenderer::ExportXML(XMLNode *xmlInfo) const
{
    UIRenderer::ExportXML(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    xmlInfo->Set("Image", imgTex ? imgTex->GetGUID() : GUID::Empty());
    xmlInfo->Set("Tint", GetTint());
}
