#include "SceneManager.h"

#include "Scene.h"
#include "Application.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
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
    sm->m_scenes.push_back(scene);
    return scene;
}

void SceneManager::AddScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); NONULL(sm);

    if (sm->m_scenes.Contains(scene))
    {
        sm->m_scenes.PushBack(scene);
    }
}

void SceneManager::SetActiveScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance(); NONULL(sm);
    if (sm->m_activeScene == scene) return;

    if (sm->m_activeScene)
    {
        sm->m_scenes.remove(sm->m_activeScene);
        delete sm->m_activeScene;
        sm->m_activeScene = nullptr;
    }

    sm->m_activeScene = scene;
    if (sm->m_activeScene)
    {
        AddScene(scene); // (it does not add repeated ones)

        #ifdef BANG_EDITOR
        Hierarchy::GetInstance()->Clear();
        #endif
    }
}

void SceneManager::SetActiveScene(const String &name)
{
    SceneManager *sm = SceneManager::GetInstance(); NONULL(sm);
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
    SceneManager *sm = SceneManager::GetInstance(); NONULL(sm);

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
