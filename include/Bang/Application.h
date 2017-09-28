#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Bang/Bang.h"
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
FORWARD class BehaviourManager;
FORWARD class ImportFilesManager;

class Application
{
public:
    Application(int argc, char **argv,
                const Path &engineRootPath = Path::Empty);
    virtual ~Application();

    Window* CreateWindow();
    DialogWindow* CreateDialogWindow(Window *parentWindow);

    void DestroyWindow(Window *window);
    int MainLoop();
    bool MainLoopIteration();

    void BlockingWait(Window *win);

    bool HandleEvents();

    Time               *GetTime() const;
    Paths              *GetPaths() const;
    ImportFilesManager *GetImportFilesManager() const;
    BehaviourManager   *GetBehaviourManager() const;
    Window             *GetCurrentWindow() const;

    virtual SceneManager* CreateSceneManager() const;
    static Window* GetTopWindow();
    const List<Window*>& GetWindows() const;

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

    static void Exit(int returnCode, bool immediate = false);

protected:
    const uint RedrawDelay_ms = 10;
    unsigned long long m_lastRenderTime = 0;

    Time               *m_time               = nullptr;
    Paths              *m_paths              = nullptr;
    ImportFilesManager *m_importFilesManager = nullptr;
    BehaviourManager   *m_behaviourManager   = nullptr;

    virtual void SetupWindow(Window *window);

private:
    static Application *s_appSingleton;

    List<Window*> m_windows;
    List<Window*> p_windowsToBeDestroyed;
    Window *p_currentWindow = nullptr;
    Window *p_latestCurrentWindow = nullptr;

    bool m_forcedExit = false;
    int m_exitCode = 0;

    void BindWindow(Window *window);
    void DestroyQueuedWindows();
};

NAMESPACE_BANG_END

#endif // APPLICATION_H
