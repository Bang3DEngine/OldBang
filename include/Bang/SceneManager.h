#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/Path.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class BehaviourManager;

class ISceneManagerListener : public virtual IEventListener
{
public:
    virtual void OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
    { (void)scene; (void)sceneFilepath; }
};

class SceneManager : public EventEmitter<ISceneManagerListener>
{
public:
    static void LoadScene(Scene *scene);
    static void LoadScene(const Path &sceneFilepath);
    static void LoadScene(const String &sceneFilepath);

    static void LoadSceneInstantly(Scene *scene);
    static void LoadSceneInstantly(const Path &sceneFilepath);

    static Scene* GetActiveScene();

    static SceneManager* GetActive();

    static void OnNewFrame(Scene *scene, bool update);
    static void Update();

    BehaviourManager *GetBehaviourManager() const;

protected:
    Scene *m_activeScene = nullptr;

    Scene *m_queuedScene = nullptr;
    Path m_queuedScenePath = Path::Empty;

    SceneManager();
    virtual ~SceneManager();

    virtual void _Update();
    virtual void _LoadSceneInstantly(Scene *scene);
    void _SetActiveScene(Scene *activeScene);
    Scene *_GetActiveScene() const;

private:
    BehaviourManager *m_behaviourManager = nullptr;

    void Init();
    virtual BehaviourManager* CreateBehaviourManager() const;

    static List<GameObject *> FindDontDestroyOnLoadGameObjects(GameObject *go);

    friend class Window;
    friend class Application;
};

NAMESPACE_BANG_END

#endif // SCENEMANAGER_H
