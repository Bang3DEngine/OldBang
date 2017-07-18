#ifndef APPLICATION_H
#define APPLICATION_H

#include "Bang/Map.h"
#include "Bang/Array.h"

class Time;
class Input;
class Scene;
class Window;
class AudioManager;
class SceneManager;
class AssetsManager;
class BehaviourManager;
class Application
{
public:
    Application();
    virtual ~Application();

    virtual void MainLoop();

    Time *GetTime() const;
    Input *GetInput() const;
    AudioManager *GetAudioManager() const;
    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    Window *GetMainWindow() const;
    void ResetDeltaTime();

protected:
    Window *m_window                     = nullptr;
    Time *m_time                         = nullptr;
    Input *m_input                       = nullptr;
    AudioManager *m_audioManager         = nullptr;
    SceneManager *m_sceneManager         = nullptr;
    AssetsManager *m_assetsManager       = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    unsigned long long m_lastRenderTime = 0;

    // If its too low, Qt stops it when dragging smthing around the screen :(
    const int c_redrawDelay = 16; // 60 FPS cap

    virtual void OnMainLoopIterationEnd();

    friend class SceneManager;
    friend class BehaviourManager;
};

#endif // APPLICATION_H
