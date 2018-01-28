#include "Bang/SceneManager.h"

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/Window.h"
#include "Bang/Extensions.h"
#include "Bang/ObjectManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    if (GetActiveScene())
    {
        GameObject::Destroy( GetActiveScene() );
    }
    delete m_behaviourManager;
}

void SceneManager::Init()
{
    m_behaviourManager = CreateBehaviourManager();
}

BehaviourManager *SceneManager::CreateBehaviourManager() const
{
    return new BehaviourManager();
}

SceneManager *SceneManager::GetInstance()
{
    Window *win = Window::GetActive();
    return win ? win->GetSceneManager() : nullptr;
}

void SceneManager::UpdateScene(Scene *scene)
{
    if (!scene) { return; }

    SceneManager::TryToLoadQueuedScene();

    scene->GetLocalObjectManager()->StartObjects();
    scene->PreUpdate();
    scene->Update();
    scene->PostUpdate();
    scene->GetLocalObjectManager()->DestroyObjects();
}

BehaviourManager *SceneManager::GetBehaviourManager() const
{
    return m_behaviourManager;
}

void SceneManager::_Update()
{
    SceneManager::UpdateScene( SceneManager::GetActiveScene() );
}

void SceneManager::Update()
{
    SceneManager::GetInstance()->_Update();
}

void SceneManager::_LoadSceneInstantly(Scene *scene)
{
    if (m_activeScene != scene)
    {
        _SetActiveScene(scene);
        if (m_activeScene)
        {
            m_activeScene->SetFirstFoundCamera();
            m_activeScene->InvalidateCanvas();
        }
    }
}

Scene *SceneManager::GetActiveScene()
{
    SceneManager *sm = SceneManager::GetInstance();
    return sm ? sm->_GetActiveScene() : nullptr;
}
Scene *SceneManager::_GetActiveScene() const { return m_activeScene; }

void SceneManager::_SetActiveScene(Scene *activeScene)
{
    if (_GetActiveScene() != activeScene)
    {
        m_activeScene = activeScene;
    }
}

void SceneManager::LoadScene(const Path &sceneFilepath)
{
    Path basePath(sceneFilepath);
    Path scenePath(basePath);
    if (!scenePath.IsFile())
    {
        if (!basePath.HasExtension(Extensions::GetSceneExtension()))
        {
            basePath = basePath.AppendExtension(Extensions::GetSceneExtension());
        }
        scenePath = basePath;

        if (!scenePath.IsFile()) { scenePath = EPATH(basePath.GetAbsolute()); }
        if (!scenePath.IsFile()) { scenePath = PPATH(basePath.GetAbsolute()); }
    }

    SceneManager *sm = SceneManager::GetInstance();
    if (scenePath.IsFile()) { sm->m_queuedSceneFilepath = scenePath; }
    else
    {
        Debug_Warn("Scene '" << scenePath << "' does not exist.");
    }
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

void SceneManager::SetActiveSceneFilepath(const Path &sceneFilepath)
{
    SceneManager *sm = SceneManager::GetInstance();
    sm->m_activeSceneFilepath = sceneFilepath;
}

void SceneManager::LoadSceneInstantly(Scene *scene)
{
    SceneManager *sm = SceneManager::GetInstance();
    sm->_LoadSceneInstantly(nullptr);

    if (scene)
    {
        sm->_LoadSceneInstantly(scene);
    }
}

void SceneManager::LoadSceneInstantly(const Path &sceneFilepath)
{
    Scene *scene = GameObjectFactory::CreateScene(false);

    SceneManager::SetActiveSceneFilepath( sceneFilepath );
    if (scene->ImportXMLFromFile(sceneFilepath))
    {
        scene->SetName(sceneFilepath.GetName());
        SceneManager::GetInstance()->
            EventEmitter<ISceneManagerListener>::PropagateToListeners(
                &ISceneManagerListener::OnSceneOpen, scene, sceneFilepath);
        SceneManager::LoadSceneInstantly(scene);
    }
    else
    {
        Debug_Error("Scene from file '" << sceneFilepath <<
                    "' could not be loaded.");
        delete scene;
    }
}

List<GameObject *> SceneManager::FindDontDestroyOnLoadGameObjects(GameObject *go)
{
    List<GameObject*> result;
    for (GameObject *child : go->GetChildren())
    {
        if (child->IsDontDestroyOnLoad()) { result.PushBack(child); }
        else { result.PushBack(FindDontDestroyOnLoadGameObjects(child)); }
    }
    return result;
}
