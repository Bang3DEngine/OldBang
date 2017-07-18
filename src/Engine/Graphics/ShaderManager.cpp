#include "Bang/ShaderManager.h"

#include "Bang/Shader.h"
#include "Bang/Application.h"
#include "Bang/FileTracker.h"
#include "Bang/G_ShaderProgram.h"

ShaderManager::ShaderManager()
{
    m_refreshTimer.start(3000);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()));

    m_lastRefreshTime = Time::GetNow();
}

ShaderManager *ShaderManager::GetInstance()
{
    return Application::GetInstance()->GetShaderManager();
}

G_Shader *ShaderManager::Load(G_Shader::Type type, const Path &filepath)
{
    ShaderManager *sm = ShaderManager::GetInstance();

    G_Shader* shader = nullptr;
    if (!sm->m_filepathToShaders.ContainsKey(filepath))
    {
        shader = new Shader(type);
        shader->LoadFromFile(filepath);
        sm->m_filepathToShaders.Set(filepath, shader);
    }
    else
    {
        shader = sm->m_filepathToShaders.Get(filepath);
    }

    return shader;
}

void ShaderManager::RegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                          G_Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    ShaderManager *sm = ShaderManager::GetInstance();
    if (!sm->m_shaderUsages.ContainsKey(shaderBeingUsed))
    {
        sm->m_shaderUsages.Set(shaderBeingUsed, Set<G_ShaderProgram*>());
    }
    sm->m_shaderUsages.Get(shaderBeingUsed).Insert(shaderProgram);
}

void ShaderManager::UnRegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                            G_Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    ShaderManager *sm = ShaderManager::GetInstance();
    ENSURE (sm->m_shaderUsages.ContainsKey(shaderBeingUsed));

    sm->m_shaderUsages.Get(shaderBeingUsed).Remove(shaderProgram);
}

void ShaderManager::Refresh()
{
}
