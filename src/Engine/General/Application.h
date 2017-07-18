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
    Application(int argc, char **argv);
    virtual ~Application();

    virtual void MainLoop();
    bool ProcessEvents();

    Input *GetInput() const;
    ShaderManager *GetShaderManager() const;
    AudioManager *GetAudioManager() const;
    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    Window *GetMainWindow() const;

protected:
    Window *m_window                     = nullptr;
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
