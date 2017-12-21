#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;

class SceneManager
{
public:
    static void LoadScene(Scene *scene);
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
    virtual void _LoadScene(Scene *scene);
    void _SetActiveScene(Scene *activeScene);
    Scene *_GetActiveScene() const;

private:
    Path m_queuedSceneFilepath;
    Path m_activeSceneFilepath;

    static List<GameObject *>
    FindDontDestroyOnLoadGameObjects(GameObject *go);
    static void TryToLoadQueuedScene();
    static void SetActiveSceneFilepath(const Path &sceneFilepath);
    static void CloseOpenScene();
    static bool IsActiveSceneSaved();
    static void OnActiveSceneSavedAs(const Path &filepath);

    friend class Window;
    friend class Application;
};

NAMESPACE_BANG_END

#endif // SCENEMANAGER_H
