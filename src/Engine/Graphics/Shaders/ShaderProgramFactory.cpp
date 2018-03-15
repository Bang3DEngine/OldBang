#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

Path ShaderProgramFactory::GetDefaultVertexShaderPath()
{
    return EPATH("Shaders/G_Default.vert");
}

Path ShaderProgramFactory::GetDefaultFragmentShaderPath()
{
    return EPATH("Shaders/G_Default.frag");
}

Path ShaderProgramFactory::GetPostProcessVertexShaderPath()
{
    return EPATH("Shaders/PP_ScreenPass.vert");
}

ShaderProgram *ShaderProgramFactory::GetDefault()
{
    return Get(ShaderProgramFactory::GetDefaultVertexShaderPath(),
               ShaderProgramFactory::GetDefaultFragmentShaderPath());
}

ShaderProgram *ShaderProgramFactory::GetDefaultPostProcess()
{
    return Get(ShaderProgramFactory::GetPostProcessVertexShaderPath(),
               EPATH("Shaders/Blur.frag"));
}

ShaderProgram *ShaderProgramFactory::Get(const Path &vShaderPath,
                                         const Path &fShaderPath)
{
    ShaderProgramFactory *spf = ShaderProgramFactory::GetActive();

    const auto &shaderPathsPair = std::make_pair(vShaderPath, fShaderPath);
    if ( !spf->m_cache.ContainsKey(shaderPathsPair) )
    {
        RH<ShaderProgram> shaderProgram =
                    Resources::Create<ShaderProgram>(vShaderPath, fShaderPath);
        spf->m_cache.Add(shaderPathsPair, shaderProgram);
    }

    return spf->m_cache.Get(shaderPathsPair).Get();
}

ShaderProgramFactory *ShaderProgramFactory::GetActive()
{
    Resources *rs = Resources::GetActive();
    return rs ? rs->m_shaderProgramFactory : nullptr;
}
