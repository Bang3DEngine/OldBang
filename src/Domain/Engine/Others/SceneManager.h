#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bang/List.h"

#include "Bang/String.h"

class Scene;
class SceneManager
{
public:

    static void SetActiveScene(Scene *scene);
    static void LoadScene(const String &sceneFilepath);

    static Scene* GetActiveScene();

    static SceneManager* GetInstance();

private:
    SceneManager();

    Scene *m_activeScene = nullptr;
    String m_queuedSceneFilepath = "";
    String m_currentSceneFilepath      = "";

    static void TryToLoadQueuedScene();
    static const String& GetOpenSceneFilepath();
    static void OpenScene(const String &filepath);
    static void CloseOpenScene();
    static bool IsCurrentSceneSaved();
    static void OnCurrentSceneSavedAs(const String &filepath);
    static void LoadSceneInstantly(const String &sceneFilepath);

    friend class MenuBar;
    friend class Explorer;
    friend class Application;
    friend class EditorWindow;
    friend class ProjectManager;
};

#endif // SCENEMANAGER_H
