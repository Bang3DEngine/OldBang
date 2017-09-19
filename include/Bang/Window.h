#ifndef WINDOW_H
#define WINDOW_H

#include "Bang/Vector2.h"

FORWARD union  SDL_Event;
FORWARD struct SDL_Window;

NAMESPACE_BANG_BEGIN

FORWARD class Time;
FORWARD class Input;
FORWARD class GEngine;
FORWARD class Texture2D;
FORWARD class AudioManager;
FORWARD class SceneManager;

class Window
{
public:
    Window();
    virtual ~Window();

    void Initialize();
    void SwapBuffers() const;
    bool MainLoopIteration();

    bool HandleEvent(const SDL_Event &sdlEvent);

    void Resize(int newWidth, int newHeight);
    void OnResize(int newWidth, int newHeight);

    int GetWidth() const;
    int GetHeight() const;
    bool HasFocus() const;
    bool IsMouseOver() const;
    Vector2i GetSize() const;
    Texture2D *GetScreenRenderTexture() const;

    static float GetAspectRatioS();
    static int GetHeightS();
    static int GetWidthS();

    GEngine      *GetGEngine() const;
    Input        *GetInput() const;
    AudioManager *GetAudioManager() const;
    SceneManager *GetSceneManager() const;

    SDL_Window *GetSDLWindow() const;
    uint GetSDLWindowID() const;
    static Window *GetCurrent();

protected:
    SDL_Window *m_sdlWindow = nullptr;

    GEngine *m_gEngine = nullptr;
    Input              *m_input              = nullptr;
    AudioManager       *m_audioManager       = nullptr;
    SceneManager       *m_sceneManager       = nullptr;
    Texture2D *m_screenRenderTexture = nullptr;
};

NAMESPACE_BANG_END

#endif // IWINDOW_H
