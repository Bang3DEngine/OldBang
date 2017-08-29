#include "Bang/SceneManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/MeshIO.h"
#include "Bang/XMLParser.h"
#include "Bang/Behaviour.h"
#include "Bang/Extensions.h"
#include "Bang/Application.h"

SceneManager::SceneManager()
{
}

SceneManager *SceneManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->GetSceneManager() : nullptr;
}

void SceneManager::Update()
{
    SceneManager::TryToLoadQueuedScene();

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->Update();
        activeScene->DestroyQueuedGameObjects();
    }
}

void SceneManager::LoadScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);
    ENSURE(sm->m_activeScene != scene);

    sm->m_activeScene = scene;
    if (sm->m_activeScene)
    {
        Time::ResetDeltaTime();
        sm->m_activeScene->Start();
        sm->m_activeScene->SetFirstFoundCameraOrDefaultOne();
        Time::ResetDeltaTime();
    }
}

Scene *SceneManager::GetActiveScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    return sm ? sm->m_activeScene : nullptr;
}

void SceneManager::LoadScene(const Path &sceneFilepath)
{
    Path spath(sceneFilepath);
    if (!spath.IsFile()) { spath = PPATH(spath.GetAbsolute()); }
    if (!spath.IsFile())
    {
        spath = spath.AppendExtension(Extensions::Get<Scene>());
    }

    SceneManager *sm = SceneManager::GetInstance();
    if (spath.IsFile()) { sm->m_queuedSceneFilepath = spath; }
    else { Debug_Warn("Scene '" << spath << "' does not exist."); }
}

void SceneManager::LoadScene(const String &sceneFilepath)
{
    SceneManager::LoadScene( Path(sceneFilepath) );
}

void SceneManager::TryToLoadQueuedScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    if (sm->m_queuedSceneFilepath.IsFile())
    {
        SceneManager::LoadSceneInstantly(sm->m_queuedSceneFilepath);
        sm->m_queuedSceneFilepath = Path::Empty;
    }
}

const Path& SceneManager::GetActiveSceneFilepath()
{
    return SceneManager::GetInstance()->m_activeSceneFilepath;
}

void SceneManager::OpenScene(const Path &filepath)
{
    ENSURE(filepath.IsFile());
    SceneManager::LoadSceneInstantly(filepath);
}

void SceneManager::SetActiveSceneFilepath(const Path &sceneFilepath)
{
    SceneManager *sm = SceneManager::GetInstance();
    sm->m_activeSceneFilepath = sceneFilepath;
}

void SceneManager::CloseOpenScene()
{
    SceneManager::SetActiveSceneFilepath( Path::Empty );
}

bool SceneManager::IsActiveSceneSaved()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (!activeScene) { return false; }

    Path openSceneFilepath = SceneManager::GetActiveSceneFilepath();
    String savedFileXML    = File::GetContents(openSceneFilepath);
    String currentSceneXML = activeScene->GetXMLInfo().ToString();
    return (savedFileXML == currentSceneXML);
}

void SceneManager::OnActiveSceneSavedAs(const Path &filepath)
{
    SceneManager::SetActiveSceneFilepath(filepath);
}

void SceneManager::LoadSceneInstantly(Scene *scene)
{
    Scene *oldScene = SceneManager::GetActiveScene();
    if (oldScene) { delete oldScene; }

    SceneManager::LoadScene(nullptr);

    if (scene)
    {
        SceneManager::LoadScene(scene);
    }
}

void SceneManager::LoadSceneInstantly(const Path &sceneFilepath)
{
    Scene *scene = new Scene();

    SceneManager::SetActiveSceneFilepath( sceneFilepath );
    if (scene->ImportXMLFromFile(sceneFilepath))
    {
        scene->SetName(sceneFilepath.GetName());
        SceneManager::LoadSceneInstantly(scene);
    }
    else
    {
        Debug_Error("Scene from file '" << sceneFilepath <<
                    "' could not be loaded.");
    }
}
