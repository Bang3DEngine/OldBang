#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/Path.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;

class SceneManagerListener : public IEventListener
{
public:
    virtual void OnSceneOpen(Scene *scene, const Path &sceneFilepath) {}
};

class SceneManager : public EventEmitter<SceneManagerListener>
{
public:
    static void LoadScene(const Path &sceneFilepath);
    static void LoadScene(const String &sceneFilepath);

    static void LoadSceneInstantly(Scene *scene);
    static void LoadSceneInstantly(const Path &sceneFilepath);

    static Scene* GetActiveScene();
    static const Path& GetActiveSceneFilepath();

    static SceneManager* GetInstance();

    static void Update();
    static void UpdateScene(Scene *scene);
protected:
    Scene *m_activeScene = nullptr;

    SceneManager();
    virtual ~SceneManager();

    virtual void _Update();
    virtual void _StartScene(Scene *scene);
    virtual void _LoadSceneInstantly(Scene *scene);
    void _SetActiveScene(Scene *activeScene);
    Scene *_GetActiveScene() const;

private:
    Path m_queuedSceneFilepath;
    Path m_activeSceneFilepath;

    static List<GameObject *> FindDontDestroyOnLoadGameObjects(GameObject *go);
    static void SetActiveSceneFilepath(const Path &sceneFilepath);
    static void TryToLoadQueuedScene();

    friend class Window;
    friend class Application;
};

NAMESPACE_BANG_END

#endif // SCENEMANAGER_H
