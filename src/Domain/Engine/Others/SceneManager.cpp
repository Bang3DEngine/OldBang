#include "Bang/SceneManager.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/Application.h"
#include "Bang/EditorWindow.h"
#include "Bang/BehaviourHolder.h"
#include "Bang/BehaviourManager.h"

#ifdef BANG_EDITOR
#include "Bang/Inspector.h"
#include "Bang/Hierarchy.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorState.h"
#endif

SceneManager::SceneManager()
{
}

SceneManager *SceneManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_sceneManager : nullptr;
}

void SceneManager::SetActiveScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);
    ENSURE(sm->m_activeScene != scene);

    if (sm->m_activeScene)
    {
        sm->m_activeScene = nullptr;
    }

    sm->m_activeScene = scene;
    if (sm->m_activeScene)
    {
        bool setCamera = true;
        #ifdef BANG_EDITOR
        Hierarchy::GetInstance()->Clear();
        setCamera = EditorState::IsPlaying();
        #endif

        Application::GetInstance()->ResetDeltaTime();
        sm->m_activeScene->_OnStart();
        if (setCamera)
        {
            sm->m_activeScene->SetFirstFoundCameraOrDefaultOne();
        }
    }
}

Scene *SceneManager::GetActiveScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    return sm ? sm->m_activeScene : nullptr;
}

void SceneManager::LoadScene(const String &sceneFilepath)
{
    SceneManager *sm = SceneManager::GetInstance();

    String spath = sceneFilepath;
    if (!IO::ExistsFile(spath))
    {
        spath = IO::ToAbsolute(spath, false);
    }

    if (!IO::ExistsFile(spath))
    {
        spath = IO::AppendExtension(
                    spath, Scene::GetFileExtensionStatic());
    }

    sm->m_queuedSceneFilepath = "";
    if (IO::ExistsFile(spath))
    {
        sm->m_queuedSceneFilepath = spath;
    }
    else
    {
        Debug_Warn("Scene '" << sceneFilepath << "' does not exist.");
    }
}

void SceneManager::TryToLoadQueuedScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    if (!sm->m_queuedSceneFilepath.Empty())
    {
        SceneManager::LoadSceneInstantly(sm->m_queuedSceneFilepath);
        sm->m_queuedSceneFilepath = "";
    }
}

const String &SceneManager::GetOpenSceneFilepath()
{
    return SceneManager::GetInstance()->m_currentSceneFilepath;
}

void SceneManager::OpenScene(const String &filepath)
{
    ENSURE(IO::ExistsFile(filepath));
    SceneManager::GetInstance()->m_currentSceneFilepath = filepath;
    SceneManager::LoadSceneInstantly(filepath);
    EditorWindow::GetInstance()->RefreshDocksAndWindowTitles();
}

void SceneManager::CloseOpenScene()
{
    SceneManager::GetInstance()->m_currentSceneFilepath = "";
}

bool SceneManager::IsCurrentSceneSaved()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (!activeScene) { return false; }

    String openSceneFilepath = SceneManager::GetOpenSceneFilepath();
    String savedFileXML = IO::GetFileContents(openSceneFilepath);
    return savedFileXML == activeScene->GetXMLInfo().ToString();
}

void SceneManager::OnCurrentSceneSavedAs(const String &filepath)
{
    SceneManager::GetInstance()->m_currentSceneFilepath = filepath;
}

void SceneManager::LoadSceneInstantly(const String &sceneFilepath)
{
    Scene *oldScene = SceneManager::GetActiveScene();
    if (oldScene) { delete oldScene; }

    #ifdef BANG_EDITOR
    Inspector::GetInstance()->Clear();
    Hierarchy::GetInstance()->Clear();
    #endif
    SceneManager::SetActiveScene(nullptr);

    #ifdef BANG_EDITOR
    EditorScene *scene = new EditorScene();
    #else
    Scene *scene = new Scene();
    #endif

    if (scene)
    {
        scene->ReadFromFile(sceneFilepath);
        SceneManager::SetActiveScene(scene);
    }
    else
    {
        Debug_Error("Scene from file '" << sceneFilepath <<
                    "' could not be loaded.");
    }
}
