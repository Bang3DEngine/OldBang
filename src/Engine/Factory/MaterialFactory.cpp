#include "Bang/MaterialFactory.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"

USING_NAMESPACE_BANG

RH<Material> MaterialFactory::GetDefault()
{
    return MaterialFactory::Load("Materials/G_Default.bmat");
}
RH<Material> MaterialFactory::GetDefaultUnLighted()
{
    return MaterialFactory::Load("Materials/G_DefaultNoPP.bmat");
}

RH<Material> MaterialFactory::GetGizmosLightedScene()
{
    return MaterialFactory::Load("Materials/G_GizmosLightedScene.bmat");
}

RH<Material> MaterialFactory::GetGizmosUnLightedScene()
{
    return MaterialFactory::Load("Materials/G_GizmosUnLightedScene.bmat");
}

RH<Material> MaterialFactory::GetGizmosUnLightedOverlay()
{
    return MaterialFactory::Load("Materials/G_GizmosUnLightedOverlay.bmat");
}

RH<Material> MaterialFactory::GetGizmosLightedOverlay()
{
    return MaterialFactory::Load("Materials/G_GizmosLightedOverlay.bmat");
}

RH<Material> MaterialFactory::GetMissing()
{
    return MaterialFactory::Load("Materials/Missing.bmat");
}

RH<Material> MaterialFactory::GetPointLight()
{
    return MaterialFactory::Load("Materials/PP_PointLight.bmat");
}
RH<Material> MaterialFactory::GetDirectionalLight()
{
    return MaterialFactory::Load("Materials/PP_DirectionalLight.bmat");
}

RH<Material> MaterialFactory::GetUIText()
{
    return MaterialFactory::Load("Materials/UI/G_UITextRenderer.bmat");
}
RH<Material> MaterialFactory::GetUIImage()
{
    return MaterialFactory::Load("Materials/UI/G_UIImageRenderer.bmat");
}

RH<Material> MaterialFactory::GetRenderTextureToViewport()
{
    return MaterialFactory::Load("Materials/RenderTextureToViewport.bmat");
}

RH<Material> MaterialFactory::Load(const String &matEnginePath)
{
    MaterialFactory *mf = MaterialFactory::GetActive();
    mf->m_cache.Add(matEnginePath, Resources::Load<Material>(EPATH(matEnginePath)));
    return mf->m_cache.Get(matEnginePath);
}

MaterialFactory *MaterialFactory::GetActive()
{
    return Resources::GetActive()->m_materialFactory;
}
