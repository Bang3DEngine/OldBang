#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Bang/Map.h"
#include "Bang/Array.h"

class Time;
class Input;
class Scene;
class Paths;
class Window;
class AudioManager;
class SceneManager;
class AssetsManager;
class ShaderManager;
class BehaviourManager;
class Application
{
public:
    Application(int argc, char **argv);
    virtual ~Application();

    virtual void CreateWindow();
    virtual void MainLoop();
    bool ProcessEvents();

    Input            *GetInput() const;
    Time             *GetTime() const;
    Paths            *GetPaths() const;
    SceneManager     *GetSceneManager() const;
    ShaderManager    *GetShaderManager() const;
    AudioManager     *GetAudioManager() const;
    AssetsManager    *GetAssetsManager() const;
    BehaviourManager *GetBehaviourManager() const;
    Window           *GetMainWindow() const;

    static Application *GetInstance();
    static void SetApplicationSingleton(Application *app);

protected:
    const int RedrawDelay_ms = 16; // 60 FPS cap
    unsigned long long m_lastRenderTime = 0;

    Window           *m_window           = nullptr;
    Time             *m_time             = nullptr;
    Paths            *m_paths            = nullptr;
    Input            *m_input            = nullptr;
    ShaderManager    *m_shaderManager    = nullptr;
    AudioManager     *m_audioManager     = nullptr;
    SceneManager     *m_sceneManager     = nullptr;
    AssetsManager    *m_assetsManager    = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

private:
    static Application *s_appSingleton;
};

#endif // APPLICATION_H
