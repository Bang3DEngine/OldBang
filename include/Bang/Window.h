#ifndef WINDOW_H
#define WINDOW_H

#include "Bang/List.h"
#include "Bang/Vector2.h"

FORWARD union  SDL_Event;
FORWARD struct SDL_Window;
using SDL_GLContext = void*;

NAMESPACE_BANG_BEGIN

FORWARD class Time;
FORWARD class Input;
FORWARD class Camera;
FORWARD class GEngine;
FORWARD class Resources;
FORWARD class Texture2D;
FORWARD class AudioManager;
FORWARD class SceneManager;
FORWARD class ObjectManager;

class Window
{
public:
    Window();
    virtual ~Window();

    virtual void Create(uint flags);
    void SwapBuffers() const;
    void MakeCurrent() const;
    bool MainLoopIteration();

    void Clear();
    virtual void Update();
    virtual void Render();
    virtual void BlitToScreen(Camera *camera);

    virtual bool HandleEvent(const SDL_Event &sdlEvent);
    void OnHandleEventsFinished();

    void MoveToFront() const;
    void SetBordered(bool bordered);
    void SetMinSize(int minSizeX, int minSizeY);
    void SetMaxSize(int maxSizeX, int maxSizeY);
    void SetResizable(bool resizable);
    void SetPosition(int newPosX, int newPosY);
    void SetTitle(const String &title);

    void SetSize(int newWidth, int newHeight);
    virtual void OnResize(int newWidth, int newHeight);

    int GetWidth() const;
    int GetHeight() const;
    bool HasFocus() const;
    bool IsBordered() const;
    String GetTitle() const;
    bool IsMouseOver() const;
    Vector2i GetSize() const;
    Vector2i GetMinSize() const;
    Vector2i GetMaxSize() const;
    bool IsResizable() const;
    Vector2i GetPosition() const;
    bool IsBlockedByChildren() const;
    bool HasFlags(uint flags) const;

    static float GetAspectRatioS();
    static int GetHeightS();
    static int GetWidthS();

    SDL_GLContext   GetGLContext() const;
    Resources      *GetResources() const;
    GEngine        *GetGEngine() const;
    Input          *GetInput() const;
    AudioManager   *GetAudioManager() const;
    SceneManager   *GetSceneManager() const;
    ObjectManager  *GetObjectManager() const;

    SDL_Window *GetSDLWindow() const;
    uint GetSDLWindowID() const;

    static void Destroy(Window *win);
    static Window *GetCurrent();

protected:
    void SetParent(Window *parentWindow);

private:
    SDL_Window *m_sdlWindow = nullptr;

    List<Window*> p_children;
    Window* p_parent = nullptr;

    SDL_GLContext   m_sdlGLContext  = nullptr;
    Resources      *m_resources     = nullptr;
    GEngine        *m_gEngine       = nullptr;
    Input          *m_input         = nullptr;
    AudioManager   *m_audioManager  = nullptr;
    SceneManager   *m_sceneManager  = nullptr;
    ObjectManager  *m_objectManager = nullptr;

    Vector2i m_minSize = Vector2i::Zero;
    Vector2i m_maxSize = Vector2i(4096);
    Vector2i m_prevSize = Vector2i::Zero;
    Vector2i m_newSize  = Vector2i::Zero;
    bool m_isResizable = true, m_resizableChanged = false;

    void _SetMinSize(int minSizeX, int minSizeY);
    void _SetMaxSize(int maxSizeX, int maxSizeY);
    Vector2i _GetMinSize() const;
    Vector2i _GetMaxSize() const;
    bool IsParentWindow(int sdlWindowId) const;

    friend int EventFilter(void *userData, SDL_Event *event);
};

NAMESPACE_BANG_END

#endif // IWINDOW_H
