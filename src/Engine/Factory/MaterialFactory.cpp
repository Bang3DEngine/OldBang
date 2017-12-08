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

RH<Material> MaterialFactory::GetMissing()
{
    return MaterialFactory::Load("Materials/Missing.bmat");
}

RH<Material> MaterialFactory::GetPointLight()
{
    return MaterialFactory::Load("Materials/PP_PointLight_Screen.bmat");
}
RH<Material> MaterialFactory::GetDirectionalLight()
{
    return MaterialFactory::Load("Materials/PP_DirectionalLight_Screen.bmat");
}

RH<Material> MaterialFactory::GetUIText()
{
    return MaterialFactory::Load("Materials/UI/G_UITextRenderer.bmat");
}
RH<Material> MaterialFactory::GetUIImage()
{
    return MaterialFactory::Load("Materials/UI/G_UIImageRenderer.bmat");
}

RH<Material> MaterialFactory::GetRenderGBufferToScreen()
{
    return MaterialFactory::Load("Materials/RenderGBufferToScreen.bmat");
}

RH<Material> MaterialFactory::Load(const String &enginePath)
{
    MaterialFactory *mf = MaterialFactory::GetActive();
    mf->m_cache.Add(enginePath, Resources::Load<Material>(EPATH(enginePath)));
    return Resources::Clone<Material>( mf->m_cache.Get(enginePath) );
}

MaterialFactory *MaterialFactory::GetActive()
{
    return Resources::GetActive()->m_materialFactory;
}
