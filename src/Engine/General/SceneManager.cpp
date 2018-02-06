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

void SceneManager::_Update()
{
    SceneManager *sm = SceneManager::GetActive();
    if (sm->GetNextLoadNeeded())  {  _LoadSceneInstantly(); }

    SceneManager::OnNewFrame( SceneManager::GetActiveScene(), true );
}

void SceneManager::Update()
{
    SceneManager::GetActive()->_Update();
}

Scene *SceneManager::GetActiveScene()
{
    SceneManager *sm = SceneManager::GetActive();
    return sm ? sm->_GetActiveScene() : nullptr;
}
Scene *SceneManager::_GetActiveScene() const { return p_activeScene; }

void SceneManager::PropagateNextSceneLoadedToListeners()
{
    EventEmitter<ISceneManagerListener>::PropagateToListeners(
        &ISceneManagerListener::OnSceneLoaded,
            GetNextLoadScene(), GetNextLoadScenePath());
}

void SceneManager::_SetActiveScene(Scene *activeScene)
{
    if (_GetActiveScene() != activeScene)
    {
        if (_GetActiveScene())
        {
            _GetActiveScene()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        p_activeScene = activeScene;
        if (_GetActiveScene())
        {
            _GetActiveScene()->EventEmitter<IDestroyListener>::RegisterListener(this);
        }
    }
}

void SceneManager::LoadScene(Scene *scene, bool destroyActive)
{
    SceneManager *sm = SceneManager::GetActive();
    sm->PrepareNextLoad(scene, Path::Empty, destroyActive);
}
void SceneManager::LoadScene(const Path &sceneFilepath, bool destroyActive)
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

        sm->PrepareNextLoad(scene, sceneFilepath, destroyActive);
    }
    else
    {
        Debug_Error("Scene '" << sceneFilepath << "' could not be loaded.");
        sm->ClearNextLoad();
    }
}

void SceneManager::LoadSceneInstantly(Scene *scene, bool destroyActive)
{
    SceneManager *sm = SceneManager::GetActive();
    SceneManager::LoadScene(scene, destroyActive);
    sm->_LoadSceneInstantly();
}
void SceneManager::LoadSceneInstantly(const Path &sceneFilepath, bool destroyActive)
{
    SceneManager *sm = SceneManager::GetActive();
    SceneManager::LoadScene(sceneFilepath, destroyActive);
    sm->_LoadSceneInstantly();
}

void SceneManager::_LoadSceneInstantly()
{
    ASSERT( GetNextLoadNeeded() );

    if (_GetActiveScene() != GetNextLoadScene())
    {
        Scene *activeScene = _GetActiveScene();
        if (GetNextLoadDestroyActive() && activeScene)
        {
            GameObject::Destroy(activeScene);
        }
        AudioManager::StopAllSounds();

        _SetActiveScene( GetNextLoadScene() );
        activeScene = _GetActiveScene();
        if (activeScene)
        {
            activeScene->SetFirstFoundCamera();
            activeScene->InvalidateCanvas();
        }

        PropagateNextSceneLoadedToListeners();
    }
    ClearNextLoad();
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
    if (object == GetNextLoadScene()) { ClearNextLoad(); }

    if (object == _GetActiveScene())
    {
        if (!GetNextLoadNeeded()) { PrepareNextLoad(nullptr, Path::Empty, false); }
        _LoadSceneInstantly();
    }
}

bool SceneManager::GetNextLoadNeeded() const { return m_nextLoadNeeded; }
Scene *SceneManager::GetNextLoadScene() const { return p_nextLoadScene; }
const Path &SceneManager::GetNextLoadScenePath() const { return m_nextLoadScenePath; }
bool SceneManager::GetNextLoadDestroyActive() const { return m_nextLoadDestroyActive; }
BehaviourManager *SceneManager::GetBehaviourManager() const { return m_behaviourManager; }

void SceneManager::ClearNextLoad()
{
    m_nextLoadNeeded = false;
    p_nextLoadScene = nullptr;
    m_nextLoadScenePath = Path::Empty;
    m_nextLoadDestroyActive = false;
}

void SceneManager::PrepareNextLoad(Scene *scene, const Path &scenePath, bool destroyActive)
{
    if (GetNextLoadScene() && GetNextLoadScene() != _GetActiveScene())
    {
        GetNextLoadScene()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
    }

    m_nextLoadNeeded = true;
    p_nextLoadScene = scene;
    m_nextLoadScenePath = scenePath;
    m_nextLoadDestroyActive = destroyActive;

    if (GetNextLoadScene())
    {
        GetNextLoadScene()->EventEmitter<IDestroyListener>::RegisterListener(this);
    }
}
