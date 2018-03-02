#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Bang/List.h"
#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Time;
FORWARD class Scene;
FORWARD class Paths;
FORWARD class Window;
FORWARD class GEngine;
FORWARD class AudioManager;
FORWARD class DialogWindow;
FORWARD class SceneManager;
FORWARD class WindowManager;
FORWARD class ImportFilesManager;

class Application
{
public:
    Application();
    virtual ~Application();

    virtual void Init(const Path &engineRootPath = Path::Empty);

    int MainLoop();
    bool MainLoopIteration();
    void BlockingWait(Window *win);

    Time               *GetTime() const;
    Paths              *GetPaths() const;
    AudioManager       *GetAudioManager() const;
    WindowManager      *GetWindowManager() const;
    ImportFilesManager *GetImportFilesManager() const;

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

    static void Exit(int returnCode, bool immediate = false);

protected:
    Time               *m_time               = nullptr;
    Paths              *m_paths              = nullptr;
    AudioManager       *m_audioManager       = nullptr;
    WindowManager      *m_windowManager      = nullptr;
    ImportFilesManager *m_importFilesManager = nullptr;

private:
    static Application *s_appSingleton;

    int m_exitCode = 0;
    bool m_forcedExit = false;

    virtual Paths* CreatePaths();
    virtual SceneManager* CreateSceneManager() const;
};

NAMESPACE_BANG_END

#endif // APPLICATION_H
