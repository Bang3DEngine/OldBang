#include "Bang/MaterialFactory.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"

USING_NAMESPACE_BANG

Material *MaterialFactory::GetDefault()
{
    return MaterialFactory::Load("Materials/G_Default.bmat");
}
Material *MaterialFactory::GetDefaultUnLighted()
{
    return MaterialFactory::Load("Materials/G_DefaultNoPP.bmat");
}

Material *MaterialFactory::GetMissing()
{
    return MaterialFactory::Load("Materials/Missing.bmat");
}

Material *MaterialFactory::GetPointLight()
{
    return MaterialFactory::Load("Materials/PP_PointLight_Screen.bmat");
}
Material *MaterialFactory::GetDirectionalLight()
{
    return MaterialFactory::Load("Materials/PP_DirectionalLight_Screen.bmat");
}

Material *MaterialFactory::GetUIText()
{
    return MaterialFactory::Load("Materials/UI/G_UITextRenderer.bmat");
}
Material *MaterialFactory::GetUIImage()
{
    return MaterialFactory::Load("Materials/UI/G_UIImageRenderer.bmat");
}

Material *MaterialFactory::GetRenderGBufferToScreen()
{
    return MaterialFactory::Load("Materials/RenderGBufferToScreen.bmat");
}

Material *MaterialFactory::Load(const String &enginePath)
{
    return Resources::Load<Material>( EPATH(enginePath) )->Clone();
}
