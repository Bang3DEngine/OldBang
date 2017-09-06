#include "Bang/ShaderManager.h"

#include "Bang/Shader.h"
#include "Bang/Application.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

ShaderManager::ShaderManager()
{
}

ShaderManager *ShaderManager::GetInstance()
{
    return Application::GetInstance()->GetShaderManager();
}

Shader *ShaderManager::Load(GL::ShaderType type, const Path &filepath)
{
    ShaderManager *sm = ShaderManager::GetInstance();

    Shader* shader = nullptr;
    if (!sm->m_filepathToShaders.ContainsKey(filepath))
    {
        shader = new Shader(type);
        shader->Import(filepath);
        sm->m_filepathToShaders.Add(filepath, shader);
    }
    else
    {
        shader = sm->m_filepathToShaders.Get(filepath);
    }

    return shader;
}

void ShaderManager::RegisterUsageOfShader(ShaderProgram *shaderProgram,
                                          Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    ShaderManager *sm = ShaderManager::GetInstance();
    if (!sm->m_shaderUsages.ContainsKey(shaderBeingUsed))
    {
        sm->m_shaderUsages.Add(shaderBeingUsed, Set<ShaderProgram*>());
    }
    sm->m_shaderUsages.Get(shaderBeingUsed).Add(shaderProgram);
}

void ShaderManager::UnRegisterUsageOfShader(ShaderProgram *shaderProgram,
                                            Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    ShaderManager *sm = ShaderManager::GetInstance();
    ENSURE (sm->m_shaderUsages.ContainsKey(shaderBeingUsed));

    sm->m_shaderUsages.Get(shaderBeingUsed).Remove(shaderProgram);
}

void ShaderManager::Refresh()
{
}
