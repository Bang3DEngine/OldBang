#include "Bang/MaterialFactory.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"

USING_NAMESPACE_BANG

void MaterialFactory::GetDefault(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/G_Default.bmat");
}
void MaterialFactory::GetDefaultUnLighted(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/G_DefaultNoPP.bmat");
}

void MaterialFactory::GetMissing(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/Missing.bmat");
}

void MaterialFactory::GetPointLight(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/PP_PointLight_Screen.bmat");
}
void MaterialFactory::GetDirectionalLight(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/PP_DirectionalLight_Screen.bmat");
}

void MaterialFactory::GetUIText(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/UI/G_UITextRenderer.bmat");
}
void MaterialFactory::GetUIImage(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/UI/G_UIImageRenderer.bmat");
}

void MaterialFactory::GetRenderGBufferToScreen(RH<Material> *material)
{
    return MaterialFactory::Load(material, "Materials/RenderGBufferToScreen.bmat");
}

void MaterialFactory::Load(RH<Material> *material, const String &enginePath)
{
    RH<Material> originalMaterial;
    Resources::Load<Material>(&originalMaterial, EPATH(enginePath));
    Resources::Clone<Material>(originalMaterial, material);
}
