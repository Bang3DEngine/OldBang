#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/Path.h"

FORWARD class Scene;

class SceneManager
{
public:
    static void SetActiveScene(Scene *scene);
    static void LoadScene(const Path &sceneFilepath);
    static void LoadScene(const String &sceneFilepath);

    static Scene* GetActiveScene();
    static const Path& GetActiveSceneFilepath();

    static SceneManager* GetInstance();

    static void Update();

private:
    SceneManager();

    Scene *m_activeScene = nullptr;
    Path m_queuedSceneFilepath;
    Path m_activeSceneFilepath;

    static void TryToLoadQueuedScene();
    static void OpenScene(const Path &filepath);
    static void SetActiveSceneFilepath(const Path &sceneFilepath);
    static void CloseOpenScene();
    static bool IsActiveSceneSaved();
    static void OnActiveSceneSavedAs(const Path &filepath);
    static void LoadSceneInstantly(Scene *scene);
    static void LoadSceneInstantly(const Path &sceneFilepath);

    friend class Project;
    friend class Application;
};

#endif // SCENEMANAGER_H
