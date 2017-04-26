#include "Bang/SceneManager.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/XMLParser.h"
#include "Bang/Behaviour.h"
#include "Bang/FileReader.h"
#include "Bang/Application.h"
#include "Bang/BehaviourManager.h"

#ifdef BANG_EDITOR
#include "Bang/Inspector.h"
#include "Bang/Hierarchy.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorState.h"
#include "Bang/EditorWindow.h"
#include "Bang/EditorPlayFlow.h"
#endif

SceneManager::SceneManager()
{
}

SceneManager *SceneManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_sceneManager : nullptr;
}

void SceneManager::Update()
{
    SceneManager::TryToLoadQueuedScene();

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        #ifdef BANG_EDITOR
        if (EditorState::IsPlaying())
        #endif
        {
            activeScene->_OnUpdate();
        }

        #ifdef BANG_EDITOR
        activeScene->_OnEditorUpdate();
        if (EditorWindow::GetInstance()->IsSceneTabActive())
        {
            EditorScene *edScene = Object::SCast<EditorScene>(activeScene);
            edScene->SetEditorCamera();
        }
        #endif

        activeScene->DestroyQueuedGameObjects();
    }
}

void SceneManager::SetActiveScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);
    ENSURE(sm->m_activeScene != scene);

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
        Application::GetInstance()->ResetDeltaTime();
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

const String& SceneManager::GetActiveSceneFilepath()
{
    return SceneManager::GetInstance()->m_activeSceneFilepath;
}

void SceneManager::OpenScene(const String &filepath)
{
    ENSURE(IO::ExistsFile(filepath));
    SceneManager::LoadSceneInstantly(filepath);
    #ifdef BANG_EDITOR
    EditorWindow::GetInstance()->RefreshDocksAndWindowTitles();
    #endif
}

void SceneManager::SetActiveSceneFilepath(const String &sceneFilepath)
{
    SceneManager *sm = SceneManager::GetInstance();
    sm->m_activeSceneFilepath = sceneFilepath;
}

void SceneManager::CloseOpenScene()
{
    SceneManager::SetActiveSceneFilepath("");
}

bool SceneManager::IsActiveSceneSaved()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (!activeScene) { return false; }

    String openSceneFilepath = SceneManager::GetActiveSceneFilepath();
    String savedFileXML    = IO::GetFileContents(openSceneFilepath);
    String currentSceneXML = activeScene->GetXMLInfo().ToString(true);
    return (savedFileXML == currentSceneXML);
}

void SceneManager::OnActiveSceneSavedAs(const String &filepath)
{
    SceneManager::SetActiveSceneFilepath(filepath);
}

void SceneManager::LoadSceneInstantly(Scene *scene)
{
    Scene *oldScene = SceneManager::GetActiveScene();
    if (oldScene) { delete oldScene; }

    #ifdef BANG_EDITOR
    Hierarchy::GetInstance()->Clear();
    Inspector::GetInstance()->Clear();
    #endif
    SceneManager::SetActiveScene(nullptr);

    if (scene)
    {
        SceneManager::SetActiveScene(scene);
    }
}

void SceneManager::LoadSceneInstantly(const String &sceneFilepath)
{
    #ifdef BANG_EDITOR
    EditorScene *scene = new EditorScene();
    #else
    Scene *scene = new Scene();
    #endif

    SceneManager::SetActiveSceneFilepath( sceneFilepath );
    if (scene->ReadFromFile(sceneFilepath))
    {
        SceneManager::LoadSceneInstantly(scene);
    }
    else
    {
        Debug_Error("Scene from file '" << sceneFilepath <<
                    "' could not be loaded.");
    }
}
