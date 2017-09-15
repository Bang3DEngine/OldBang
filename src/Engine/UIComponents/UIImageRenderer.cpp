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

USING_NAMESPACE_BANG

UIImageRenderer::UIImageRenderer()
{
    SetMaterial( MaterialFactory::GetUIImage() );
    UseMaterialCopy();

    p_quadMesh = MeshFactory::GetUIPlane();
}

UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::OnUpdate()
{
    UIRenderer::OnUpdate();
    UpdateQuadUvsToMatchAspectRatioMode();
}

void UIImageRenderer::OnRender()
{
    UIRenderer::OnRender();
    GL::Render(p_quadMesh->GetVAO(),
               GetRenderPrimitive(),
               p_quadMesh->GetVertexCount());
}

void UIImageRenderer::SetImageTexture(const Path &imagePath)
{
    Texture2D *tex = Resources::Load<Texture2D>(imagePath);
    SetImageTexture(tex);
}

void UIImageRenderer::SetImageTexture(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
    GetMaterial()->SetTexture(m_imageTexture);
}

void UIImageRenderer::SetTint(const Color &tint)
{
    GetMaterial()->SetDiffuseColor(tint);
}

void UIImageRenderer::SetAspectRatioMode(AspectRatioMode arMode)
{
    m_aspectRatioMode = arMode;
}

const Color &UIImageRenderer::GetTint() const
{
    return GetMaterial()->GetDiffuseColor();
}

Texture2D *UIImageRenderer::GetImageTexture() const
{
    return m_imageTexture;
}

AspectRatioMode UIImageRenderer::GetAspectRatioMode() const
{
    return m_aspectRatioMode;
}

void UIImageRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImageRenderer *img = SCAST<UIImageRenderer*>(clone);
    img->SetImageTexture( GetImageTexture() );
    img->SetAspectRatioMode( GetAspectRatioMode() );
}

void UIImageRenderer::ImportXML(const XMLNode &xmlInfo)
{
    UIRenderer::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Image"))
    { SetImageTexture( Resources::Load<Texture2D>( xmlInfo.Get<GUID>("Image") ) ); }

    if (xmlInfo.Contains("Tint"))
    { SetTint( xmlInfo.Get<Color>("Tint") ); }

    if (xmlInfo.Contains("AspectRatioMode"))
    { SetAspectRatioMode( xmlInfo.Get<AspectRatioMode>("AspectRatioMode") ); }
}

void UIImageRenderer::ExportXML(XMLNode *xmlInfo) const
{
    UIRenderer::ExportXML(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    xmlInfo->Set("Image", imgTex ? imgTex->GetGUID() : GUID::Empty());
    xmlInfo->Set("Tint", GetTint());
    xmlInfo->Set("AspectRatioMode", GetAspectRatioMode());
}

void UIImageRenderer::UpdateQuadUvsToMatchAspectRatioMode()
{
    ENSURE(m_imageTexture);
    ENSURE(m_imageTexture->GetSize() != m_prevImageTextureSize);

    RectTransform *rt = gameObject->GetComponent<RectTransform>();
    Recti rectPx = rt->GetScreenSpaceRectPx();
    ENSURE(m_prevRectSize != rectPx.GetSize());

    Array<Vector2> quadUvs;

    Vector2 texSize(m_imageTexture->GetSize());
    Vector2 texTargetSize(AspectRatio::GetAspectRatioedSize(
                                         rectPx.GetSize(), Vector2i(texSize),
                                         GetAspectRatioMode())
                          );

    Vector2 uvs = texTargetSize / texSize;
    quadUvs = {Vector2(    0, 0),
               Vector2(uvs.x, 0),
               Vector2(uvs.x, uvs.y),

               Vector2(    0, 0),
               Vector2(uvs.x, uvs.y),
               Vector2(    0, uvs.y)};

    p_quadMesh->LoadUvs(quadUvs);

    m_prevImageTextureSize = m_imageTexture->GetSize();
}
