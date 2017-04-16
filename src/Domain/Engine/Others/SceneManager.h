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
    static const String& GetActiveSceneFilepath();

    static SceneManager* GetInstance();

    static void Update();

private:
    SceneManager();

    Scene *m_activeScene          = nullptr;
    String m_queuedSceneFilepath  = "";
    String m_activeSceneFilepath  = "";

    static void TryToLoadQueuedScene();
    static void OpenScene(const String &filepath);
    static void SetActiveSceneFilepath(const String &sceneFilepath);
    static void CloseOpenScene();
    static bool IsActiveSceneSaved();
    static void OnActiveSceneSavedAs(const String &filepath);
    static void LoadSceneInstantly(Scene *scene);
    static void LoadSceneInstantly(const String &sceneFilepath);

    friend class MenuBar;
    friend class Explorer;
    friend class Application;
    friend class EditorWindow;
    friend class ProjectManager;
    friend class EditorPlayStopFlowController;
};

#endif // SCENEMANAGER_H
