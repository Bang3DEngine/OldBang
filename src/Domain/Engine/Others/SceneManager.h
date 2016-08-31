#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "List.h"

#include "String.h"

class Scene;
class SceneManager
{
friend class Application;

private:
    SceneManager();

    List<Scene*> m_scenes;
    Scene *m_activeScene = nullptr;

public:

    static Scene* AddScene(const String &name);
    static void AddScene(Scene* scene);
    static void SetActiveScene(Scene *scene);
    static void SetActiveScene(const String &name);
    static void RemoveScene(const String &name);

    static Scene* GetActiveScene();
    static Scene* GetScene(const String &name);

    static SceneManager* GetInstance();
};

#endif // SCENEMANAGER_H
