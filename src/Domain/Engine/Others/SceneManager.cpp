#include "Bang/SceneManager.h"

#include "Bang/Paths.h"
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

void SceneManager::LoadScene(const Path &sceneFilepath)
{
    SceneManager *sm = SceneManager::GetInstance();

    Path spath(sceneFilepath);
    if (!spath.IsFile()) { spath = UPATH(spath.GetAbsolute()); }
    if (!spath.IsFile())
    {
        spath = spath.AppendExtension(Scene::GetFileExtensionStatic());
    }

    if (spath.IsFile()) { sm->m_queuedSceneFilepath = spath; }
    else { Debug_Warn("Scene '" << spath << "' does not exist."); }
}

void SceneManager::LoadScene(const String &sceneFilepath)
{
    String sceneExt = Scene::GetFileExtensionStatic();
    SceneManager::LoadScene( UPATH(sceneFilepath).AppendExtension(sceneExt) );
}

void SceneManager::TryToLoadQueuedScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    if (sm->m_queuedSceneFilepath.IsFile())
    {
        SceneManager::LoadSceneInstantly(sm->m_queuedSceneFilepath);
        sm->m_queuedSceneFilepath = Path();
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
    #ifdef BANG_EDITOR
    EditorWindow::GetInstance()->RefreshDocksAndWindowTitles();
    #endif
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
    String currentSceneXML = activeScene->GetXMLInfo().ToString(true);
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

void SceneManager::LoadSceneInstantly(const Path &sceneFilepath)
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
