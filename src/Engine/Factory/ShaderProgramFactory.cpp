#include "Bang/ShaderProgramFactory.h"

#include <algorithm>

#include "Bang/Map.h"
#include "Bang/Shader.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

ShaderProgram *ShaderProgramFactory::CreateEmptyProgram()
{
    ShaderProgram *sp = Resources::Create<ShaderProgram>();
    return sp;
}

ShaderProgram *ShaderProgramFactory::GetShaderProgram(const Path &vShaderPath,
                                                      const Path &fShaderPath)
{
    Shader *vShader = Resources::Load<Shader>(vShaderPath);
    Shader *fShader = Resources::Load<Shader>(fShaderPath);
    return ShaderProgramFactory::GetShaderProgram(vShader, fShader);
}

ShaderProgram *ShaderProgramFactory::GetShaderProgram(Shader *vShader,
                                                      Shader *fShader)
{
    static Map< std::pair<Shader*, Shader*>, ShaderProgram* > spShaderCache;

    ShaderProgram *shaderProgram = nullptr;

    std::pair<Shader*, Shader*> shadersPair(vShader, fShader);
    if (spShaderCache.ContainsKey(shadersPair))
    {
        shaderProgram = spShaderCache.Get(shadersPair);
    }
    else
    {
        shaderProgram = ShaderProgramFactory::CreateEmptyProgram();
        if (shaderProgram->Load(vShader, fShader))
        { spShaderCache.Add(shadersPair, shaderProgram); }
    }

    return shaderProgram;
}
