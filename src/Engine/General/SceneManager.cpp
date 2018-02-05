#include "Bang/SceneManager.h"

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/Window.h"
#include "Bang/Extensions.h"
#include "Bang/AudioManager.h"
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

SceneManager *SceneManager::GetActive()
{
    Window *win = Window::GetActive();
    return win ? win->GetSceneManager() : nullptr;
}

void SceneManager::OnNewFrame(Scene *scene, bool update)
{
    if (scene)
    {
        scene->PreStart();
        scene->Start();

        if (update)
        {
            scene->PreUpdate();
            scene->Update();
            scene->PostUpdate();
        }

        scene->DestroyPending();
    }
}

BehaviourManager *SceneManager::GetBehaviourManager() const
{
    return m_behaviourManager;
}

void SceneManager::_Update()
{
    SceneManager *sm = SceneManager::GetActive();
    if (sm->m_queuedScene) { SceneManager::LoadSceneInstantly(sm->m_queuedScene); }

    SceneManager::OnNewFrame( SceneManager::GetActiveScene(), true );
}

void SceneManager::Update()
{
    SceneManager::GetActive()->_Update();
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
    SceneManager *sm = SceneManager::GetActive();
    return sm ? sm->_GetActiveScene() : nullptr;
}
Scene *SceneManager::_GetActiveScene() const { return m_activeScene; }

void SceneManager::_SetActiveScene(Scene *activeScene)
{
    if (_GetActiveScene() != activeScene)
    {
        if (_GetActiveScene())
        {
            _GetActiveScene()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        m_activeScene = activeScene;
        if (_GetActiveScene())
        {
            _GetActiveScene()->EventEmitter<IDestroyListener>::RegisterListener(this);
        }
    }
}

void SceneManager::LoadScene(Scene *scene)
{
    SceneManager *sm = SceneManager::GetActive();

    if (sm->m_queuedScene)
    {
        sm->m_queuedScene->EventEmitter<IDestroyListener>::UnRegisterListener(sm);
        sm->m_queuedScenePath = Path::Empty;
        GameObject::Destroy(sm->m_queuedScene);
    }

    sm->m_queuedScene = scene;
    if (sm->m_queuedScene)
    {
        sm->m_queuedScene->EventEmitter<IDestroyListener>::RegisterListener(sm);
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

    SceneManager *sm = SceneManager::GetActive();
    if (scenePath.IsFile())
    {
        Scene *scene = GameObjectFactory::CreateScene(false);
        scene->ImportXMLFromFile(sceneFilepath);

        SceneManager::LoadScene(scene);
        sm->m_queuedScenePath = sceneFilepath;
    }
    else
    {
        Debug_Error("Scene '" << sceneFilepath << "' could not be loaded.");
    }
}

void SceneManager::LoadScene(const String &sceneFilepath)
{
    SceneManager::LoadScene( Path(sceneFilepath) );
}

void SceneManager::LoadSceneInstantly(Scene *scene)
{
    SceneManager *sm = SceneManager::GetActive();

    sm->_LoadSceneInstantly(nullptr);
    if (scene) { sm->_LoadSceneInstantly(scene); }

    SceneManager::GetActive()->
        EventEmitter<ISceneManagerListener>::PropagateToListeners(
            &ISceneManagerListener::OnSceneLoaded,
                scene, sm->m_queuedScenePath);

    if (scene == sm->m_queuedScene)
    {
        sm->m_queuedScene = nullptr;
        sm->m_queuedScenePath = Path::Empty;
    }
}

void SceneManager::LoadSceneInstantly(const Path &sceneFilepath)
{
    SceneManager::LoadScene(sceneFilepath);

    SceneManager *sm = SceneManager::GetActive();
    if (sm->m_queuedScene) { SceneManager::LoadSceneInstantly(sm->m_queuedScene); }
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

void SceneManager::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    if (object == m_queuedScene)
    {
        m_queuedScene = nullptr;
        m_queuedScenePath =  Path::Empty;
    }

    if (object == m_activeScene) { _LoadSceneInstantly(nullptr); }
}
