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
class ShaderManager;
class Application
{
public:
    Application();
    virtual ~Application();

    virtual void MainLoop();

    Time *GetTime() const;
    Input *GetInput() const;
    ShaderManager *GetShaderManager() const;
    AudioManager *GetAudioManager() const;
    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    Window *GetMainWindow() const;
    void ResetDeltaTime();

protected:
    Window *m_window                     = nullptr;
    Time *m_time                         = nullptr;
    Input *m_input                       = nullptr;
    ShaderManager *m_shaderManager       = nullptr;
    AudioManager *m_audioManager         = nullptr;
    SceneManager *m_sceneManager         = nullptr;
    AssetsManager *m_assetsManager       = nullptr;

    unsigned long long m_lastRenderTime = 0;
    const int c_redrawDelay = 16; // 60 FPS cap

private:
    static Application *s_appSingleton;

    friend class SceneManager;
    friend class BehaviourManager;
};

#endif // APPLICATION_H
