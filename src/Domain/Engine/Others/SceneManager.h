#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "List.h"

#include "String.h"

class Scene;
class SceneManager
{
public:

    static Scene* AddScene(const String &name);
    static void AddScene(Scene* scene);
    static void SetActiveScene(Scene *scene);
    static void SetActiveScene(const String &name);
    static void RemoveScene(const String &name);
    static void LoadScene(const String &sceneFilepath);

    static Scene* GetActiveScene();
    static Scene* GetScene(const String &name);

    static SceneManager* GetInstance();

private:
    SceneManager();

    List<Scene*> m_scenes;
    Scene *m_activeScene = nullptr;
    String m_queuedSceneFilepath = "";

    static void TryToLoadQueuedScene();
    static void LoadSceneInstantly(const String &sceneFilepath);

    friend class MenuBar;
    friend class Explorer;
    friend class Application;
    friend class ProjectManager;
};

#endif // SCENEMANAGER_H
