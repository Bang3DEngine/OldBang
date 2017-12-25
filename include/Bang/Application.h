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
FORWARD class ImportFilesManager;

class Application
{
public:
    Application();
    virtual ~Application();

    virtual void Init(const Path &engineRootPath = Path::Empty);

    Window* CreateWindow();
    DialogWindow* CreateDialogWindow(Window *parentWindow);
    void DestroyWindow(Window *window);

    int MainLoop();
    bool MainLoopIteration();

    void BlockingWait(Window *win, Window *previousWindow = nullptr);

    bool HandleEvents();

    Time               *GetTime() const;
    Paths              *GetPaths() const;
    ImportFilesManager *GetImportFilesManager() const;

    virtual SceneManager* CreateSceneManager() const;
    const List<Window*>& GetWindows() const;
    static Window* GetMainWindow();

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

    static void Exit(int returnCode, bool immediate = false);

protected:
    unsigned long long m_lastRenderTime = 0;

    Time               *m_time               = nullptr;
    Paths              *m_paths              = nullptr;
    ImportFilesManager *m_importFilesManager = nullptr;

    void SetupWindow(Window *window);

    virtual Paths* CreatePaths();
    virtual Window* _CreateWindow();

private:
    static Application *s_appSingleton;

    List<Window*> m_windows;
    List<Window*> p_windowsToBeDestroyed;

    bool m_forcedExit = false;
    int m_exitCode = 0;

    void DestroyQueuedWindows();
};

NAMESPACE_BANG_END

#endif // APPLICATION_H
