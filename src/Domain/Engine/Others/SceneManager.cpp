#include "Bang/SceneManager.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/IO.h"
#include "Bang/Application.h"
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

Scene *SceneManager::AddScene(const String &name)
{
    SceneManager *sm = SceneManager::GetInstance();
    if (!sm) { return nullptr; }

    Scene *scene = new Scene();
    scene->m_name = name;
    sm->m_scenes.PushBack(scene);
    return scene;
}

void SceneManager::AddScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);

    if (sm->m_scenes.Contains(scene))
    {
        sm->m_scenes.PushBack(scene);
    }
}

void SceneManager::SetActiveScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);
    ENSURE(sm->m_activeScene != scene);

    if (sm->m_activeScene)
    {
        sm->m_scenes.Remove(sm->m_activeScene);
        sm->m_activeScene = nullptr;
    }

    sm->m_activeScene = scene;
    if (sm->m_activeScene)
    {
        AddScene(sm->m_activeScene); // (it does not add repeated ones)

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

void SceneManager::SetActiveScene(const String &name)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);
    for (Scene *scene : sm->m_scenes)
    {
        if (scene->name == name)
        {
            SceneManager::SetActiveScene(scene);
            return;
        }
    }
    Debug_Warn("Could not change Scene to '" << name << "', "<<
                "because no scene with this name is added to the SceneManager.");
}

Scene *SceneManager::GetActiveScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    return sm ? sm->m_activeScene : nullptr;
}

Scene *SceneManager::GetScene(const String &name)
{
    SceneManager *sm = SceneManager::GetInstance();
    if (!sm) { return nullptr; }

    for (Scene *scene : sm->m_scenes)
    {
        if (scene->name == name)
        {
            return scene;
        }
    }
    return nullptr;
}

void SceneManager::RemoveScene(const String &name)
{
    SceneManager *sm = SceneManager::GetInstance(); ENSURE(sm);

    for (auto it = sm->m_scenes.Begin();
         it != sm->m_scenes.End(); ++it)
    {
        Scene *scene = (*it);
        if (scene->name == name)
        {
            sm->m_scenes.Remove(it);
            return;
        }
    }
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
        SceneManager::AddScene(scene);
        SceneManager::SetActiveScene(scene);
        IO::SetActiveSceneFilepath(sceneFilepath);
    }
    else
    {
        Debug_Error("Scene from file '" << sceneFilepath << "' could not be loaded.");
    }
}
