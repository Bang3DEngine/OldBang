#include "G_ShaderManager.h"

#include "Bang/FileTracker.h"
#include "Bang/G_ShaderProgram.h"
#include "Bang/SingletonManager.h"

G_ShaderManager::G_ShaderManager()
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

G_ShaderManager *G_ShaderManager::GetInstance()
{
    if (!SingletonManager::Exists<G_ShaderManager>())
    {
        SingletonManager::Set<G_ShaderManager>( new G_ShaderManager() );
    }
    return SingletonManager::Get<G_ShaderManager>();
}

G_Shader *G_ShaderManager::Load(G_Shader::Type type, const Path &filepath)
{
    G_ShaderManager *sm = G_ShaderManager::GetInstance();

    G_Shader* shader = nullptr;
    if (!sm->m_filepathToShaders.ContainsKey(filepath))
    {
        shader = new G_Shader(type, filepath);
        sm->m_filepathToShaders.Set(filepath, shader);
    }
    else
    {
        shader = sm->m_filepathToShaders.Get(filepath);
    }

    return shader;
}

void G_ShaderManager::RegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                          G_Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    G_ShaderManager *sm = G_ShaderManager::GetInstance();
    if (!sm->m_shaderUsages.ContainsKey(shaderBeingUsed))
    {
        sm->m_shaderUsages.Set(shaderBeingUsed, Set<G_ShaderProgram*>());
    }
    sm->m_shaderUsages.Get(shaderBeingUsed).Insert(shaderProgram);
}

void G_ShaderManager::UnRegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                            G_Shader *shaderBeingUsed)
{
    ENSURE(shaderProgram); ENSURE(shaderBeingUsed);

    G_ShaderManager *sm = G_ShaderManager::GetInstance();
    ENSURE (sm->m_shaderUsages.ContainsKey(shaderBeingUsed));

    sm->m_shaderUsages.Get(shaderBeingUsed).Remove(shaderProgram);
}

void G_ShaderManager::Refresh()
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
