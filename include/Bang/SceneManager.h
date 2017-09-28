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

    static Scene *GetRootScene();
    static Scene* GetActiveScene();
    static const Path& GetActiveSceneFilepath();

    static SceneManager* GetInstance();

    static void Update();

protected:
    Scene *m_activeScene = nullptr;

    SceneManager();
    virtual ~SceneManager();

    virtual void _LoadScene(Scene *scene);
    virtual Scene* _GetRootScene() const;
    virtual Scene* _GetActiveScene() const;

private:
    Path m_queuedSceneFilepath;
    Path m_activeSceneFilepath;

    static List<GameObject *>
    FindDontDestroyOnLoadGameObjects(GameObject *go);
    static void TryToLoadQueuedScene();
    static void OpenScene(const Path &filepath);
    static void SetActiveSceneFilepath(const Path &sceneFilepath);
    static void CloseOpenScene();
    static bool IsActiveSceneSaved();
    static void OnActiveSceneSavedAs(const Path &filepath);
    static void LoadSceneInstantly(Scene *scene);
    static void LoadSceneInstantly(const Path &sceneFilepath);

    friend class Window;
    friend class Project;
    friend class Application;
};

NAMESPACE_BANG_END

#endif // SCENEMANAGER_H
