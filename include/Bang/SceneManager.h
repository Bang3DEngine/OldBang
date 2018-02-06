#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/Path.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class BehaviourManager;

class ISceneManagerListener : public virtual IEventListener
{
    EVENTLISTENER(ISceneManagerListener)

public:
    virtual void OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
    { (void)scene; (void)sceneFilepath; }
};

class SceneManager : public EventEmitter<ISceneManagerListener>,
                     public IDestroyListener
{
public:
    static void LoadScene(Scene *scene, bool destroyActive = true);
    static void LoadScene(const Path &sceneFilepath, bool destroyActive = true);
    static void LoadScene(const String &sceneFilepath, bool destroyActive = true);

    static void LoadSceneInstantly(Scene *scene, bool destroyActive = true);
    static void LoadSceneInstantly(const Path &sceneFilepath,
                                   bool destroyActive = true);

    static Scene* GetActiveScene();

    static SceneManager* GetActive();

    static void OnNewFrame(Scene *scene, bool update);
    static void Update();

    BehaviourManager *GetBehaviourManager() const;

protected:
    SceneManager();
    virtual ~SceneManager();

    bool GetNextLoadNeeded() const;
    Scene* GetNextLoadScene() const;
    const Path& GetNextLoadScenePath() const;
    bool GetNextLoadDestroyActive() const;

    void _SetActiveScene(Scene *activeScene);
    Scene *_GetActiveScene() const;

    virtual void _Update();
    virtual void _LoadSceneInstantly();

    void PropagateNextSceneLoadedToListeners();
    void ClearNextLoad();

private:
    BehaviourManager *m_behaviourManager = nullptr;

    Scene *p_activeScene = nullptr;

    bool m_nextLoadNeeded = false;
    Scene *p_nextLoadScene = nullptr;
    bool m_nextLoadDestroyActive = false;
    Path m_nextLoadScenePath = Path::Empty;

    void Init();
    virtual BehaviourManager* CreateBehaviourManager() const;

    void PrepareNextLoad(Scene *scene, const Path &scenePath, bool destroyActive);
    static List<GameObject *> FindDontDestroyOnLoadGameObjects(GameObject *go);

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    friend class Window;
    friend class Application;
};

NAMESPACE_BANG_END

#endif // SCENEMANAGER_H
