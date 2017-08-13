#include "Bang/UIImageRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/MeshFactory.h"
#include "Bang/Resources.h"
#include "Bang/MaterialFactory.h"

UIImageRenderer::UIImageRenderer()
{
    SetMaterial( MaterialFactory::GetUIImage() );
    UseMaterialCopy();

    p_quadMesh = MeshFactory::GetUIPlane();

    m_imageTexture = new Texture2D(); // TODO: MEMLEAK
    m_imageTexture->CreateEmpty(1,1);
}

UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::OnRender()
{
    UIRenderer::OnRender();
    GL::Render(p_quadMesh->GetVAO(),
               GetRenderPrimitive(),
               p_quadMesh->GetVertexCount());
}

void UIImageRenderer::SetImage(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
    GetMaterial()->SetTexture(m_imageTexture);
}

void UIImageRenderer::SetTint(const Color &tint)
{
    GetMaterial()->SetDiffuseColor(tint);
}

const Color &UIImageRenderer::GetTint() const
{
    return GetMaterial()->GetDiffuseColor();
}

Texture2D *UIImageRenderer::GetImageTexture() const
{
    return m_imageTexture;
}

void UIImageRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImageRenderer *img = SCAST<UIImageRenderer*>(clone);
    img->SetImage( GetImageTexture() );
}

void UIImageRenderer::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);
    Path texFilepath = xmlInfo.Get<Path>("Image");
    SetImage( Resources::Load<Texture2D>(texFilepath) );
}

void UIImageRenderer::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    Path texFilepath = imgTex ? imgTex->GetFilepath() : Path();
    xmlInfo->Set("Image", texFilepath);
}
