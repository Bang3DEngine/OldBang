#include "Bang/ShaderManager.h"

#include "Bang/Shader.h"
#include "Bang/FileTracker.h"
#include "Bang/G_ShaderProgram.h"
#include "Bang/SingletonManager.h"

ShaderManager::ShaderManager()
{
    m_refreshTimer.start(3000);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()));

    #ifdef BANG_EDITOR
    FileTracker::TrackFilesWithExtension("glsl");
    FileTracker::TrackFilesWithExtension("vert_g");
    FileTracker::TrackFilesWithExtension("vert_pp");
    FileTracker::TrackFilesWithExtension("frag_g");
    FileTracker::TrackFilesWithExtension("frag_pp");
    #endif

    m_lastRefreshTime = Time::GetNow();
}

ShaderManager *ShaderManager::GetInstance()
{
    if (!SingletonManager::Exists<ShaderManager>())
    {
        SingletonManager::Set<ShaderManager>( new ShaderManager() );
    }
    return SingletonManager::Get<ShaderManager>();
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
    #ifdef BANG_EDITOR
    List<Path> shaderFilepaths = m_filepathToShaders.GetKeys();
    for (const Path& shaderFilepath : shaderFilepaths)
    {
        if (shaderFilepath.Exists() &&
            FileTracker::HasFileChanged(shaderFilepath, m_lastRefreshTime))
        {
            G_Shader *shader = m_filepathToShaders.Get(shaderFilepath);
            shader->LoadFromFile(shaderFilepath);

            const Set<G_ShaderProgram*>& programsAffected =
                                                m_shaderUsages.Get(shader);
            for (G_ShaderProgram *shaderProgram : programsAffected)
            {
                shaderProgram->Refresh();
            }
        }
    }
    m_lastRefreshTime = Time::GetNow();
    #endif
}
