#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

ShaderProgramFactory::ShaderProgramFactory()
{
}

ShaderProgram *ShaderProgramFactory::GetDefault()
{
    return Get(EPATH("Shaders/G_Default.vert"),
               EPATH("Shaders/G_Default.frag"));
}

ShaderProgram *ShaderProgramFactory::Get(const Path &vShaderPath,
                                         const Path &fShaderPath)
{
    ShaderProgramFactory *spf = ShaderProgramFactory::GetActive();

    auto shaderPathsPair = std::make_pair(vShaderPath, fShaderPath);
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
