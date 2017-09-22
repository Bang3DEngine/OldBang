#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Bang/Bang.h"
#include "Bang/Path.h"

using SDL_GLContext = void*;

NAMESPACE_BANG_BEGIN

FORWARD class Time;
FORWARD class Scene;
FORWARD class Paths;
FORWARD class Window;
FORWARD class GEngine;
FORWARD class Resources;
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

    void RemoveWindow(Window *window);
    int MainLoop();

    bool HandleEvents();

    SDL_GLContext       GetSharedGLContext() const;
    Time               *GetTime() const;
    Paths              *GetPaths() const;
    Resources          *GetResources() const;
    ImportFilesManager *GetImportFilesManager() const;
    BehaviourManager   *GetBehaviourManager() const;
    Window             *GetCurrentWindow() const;

    static Window* GetTopWindow();
    const List<Window*>& GetWindows() const;

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

    static void Exit(int returnCode, bool immediate = false);

protected:
    const uint RedrawDelay_ms = 10;
    unsigned long long m_lastRenderTime = 0;

    SDL_GLContext       m_sdlGLSharedContext = nullptr;
    Time               *m_time               = nullptr;
    Paths              *m_paths              = nullptr;
    Resources          *m_resources          = nullptr;
    ImportFilesManager *m_importFilesManager = nullptr;
    BehaviourManager   *m_behaviourManager   = nullptr;

    virtual void SetupWindow(Window *window);

private:
    static Application *s_appSingleton;

    List<Window*> m_windows;
    Window *p_currentWindow = nullptr;

    bool m_exit = false;
    int m_exitCode = 0;
};

NAMESPACE_BANG_END

#endif // APPLICATION_H
