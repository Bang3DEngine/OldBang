#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Bang/Bang.h"
#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Time;
FORWARD class Input;
FORWARD class Scene;
FORWARD class Paths;
FORWARD class Window;
FORWARD class GEngine;
FORWARD class Resources;
FORWARD class AudioManager;
FORWARD class SceneManager;
FORWARD class BehaviourManager;
FORWARD class ImportFilesManager;

class Application
{
public:
    Application(int argc, char **argv,
                const Path &engineRootPath = Path::Empty);
    virtual ~Application();

    virtual void CreateWindow();
    virtual int MainLoop();
    virtual bool MainLoopIteration();

    virtual void UpdateScene();
    virtual void RenderScene();

    bool ProcessEvents();

    void OnResize(int newWidth, int newHeight);

    GEngine            *GetGEngine() const;
    Input              *GetInput() const;
    Time               *GetTime() const;
    Paths              *GetPaths() const;
    SceneManager       *GetSceneManager() const;
    AudioManager       *GetAudioManager() const;
    Resources          *GetResources() const;
    ImportFilesManager *GetImportFilesManager() const;
    BehaviourManager   *GetBehaviourManager() const;
    Window             *GetWindow() const;

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

    static void Exit(int returnCode, bool immediate = false);

protected:
    const int RedrawDelay_ms = 10;
    unsigned long long m_lastRenderTime = 0;

    GEngine            *m_gEngine            = nullptr;
    Window             *m_window             = nullptr;
    Time               *m_time               = nullptr;
    Paths              *m_paths              = nullptr;
    Input              *m_input              = nullptr;
    AudioManager       *m_audioManager       = nullptr;
    SceneManager       *m_sceneManager       = nullptr;
    Resources          *m_resources          = nullptr;
    ImportFilesManager *m_importFilesManager = nullptr;
    BehaviourManager   *m_behaviourManager   = nullptr;

private:
    static Application *s_appSingleton;

    bool m_exit = false;
    int m_exitCode = 0;
};

NAMESPACE_BANG_END

#endif // APPLICATION_H
