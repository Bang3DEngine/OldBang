#include "Bang/WindowManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bang/Debug.h"
#include "Bang/Window.h"
#include "Bang/Application.h"
#include "Bang/DialogWindow.h"

USING_NAMESPACE_BANG

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
    for (Window *w : GetCurrentWindows()) { delete w; }
    for (Window *w : p_windowsToBeDestroyed) { delete w; }

    TTF_Quit();
    SDL_Quit();
}

void WindowManager::Init()
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) { Debug_Error("Failed to init SDL"); }
    if ( TTF_Init() )
    {
        Debug_Error("Could not init FreeType library: Error(" <<
                    TTF_GetError() <<  ")");
    }

    m_windowsStack.push( {} );
}

bool WindowManager::MainLoopIteration()
{
    bool exit = false;

    if (!HandleEvents())        { exit = true; }
    if (GetCurrentWindows().IsEmpty()) { exit = true; }
    DestroyQueuedWindows();

    for (Window *w : GetCurrentWindows())
    {
        Window::SetActive(w);
        w->MainLoopIteration();
        Window::SetActive(nullptr);
    }

    return exit;
}

void WindowManager::OnBlockingWaitBegin(Window *win)
{
    GetCurrentWindows().Remove(win);

    m_windowsStack.push( {win} );
    Window::SetActive(win);
}

void WindowManager::OnBlockingWaitEnd()
{
    m_windowsStack.pop();
    Window::SetActive( GetCurrentWindows().Front() );
}

void WindowManager::DestroyWindow(Window *window)
{
    p_windowsToBeDestroyed.PushBack(window);
}

DialogWindow *WindowManager::CreateDialogWindow(Window *parentWindow, bool resizable)
{
    DialogWindow *w = new DialogWindow(parentWindow, resizable);
    SetupWindow(w, 0);
    return w;
}

Window *WindowManager::GetTopWindow()
{
    WindowManager *wm = WindowManager::GetInstance();
    if (!wm) { return nullptr; }
    return !wm->GetCurrentWindows().IsEmpty() ? wm->GetCurrentWindows().Front() : nullptr;
}
void WindowManager::SetupWindow(Window *window, uint _flags)
{
    WindowManager::GetInstance()->GetCurrentWindows().PushBack(window);

    uint flags = (_flags > 0 ?_flags : (SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL));
    window->Create(flags);
    window->OnResize(window->GetWidth(), window->GetHeight());
}

bool WindowManager::HandleEvents()
{
    SDL_Event sdlEvent;
    constexpr int AreThereMoreEvents = 1;
    while (SDL_PollEvent(&sdlEvent) == AreThereMoreEvents)
    {
        switch (sdlEvent.type)
        {
            default:
            {
                List<Window*> windowsToBeClosed;
                for (Window *w : GetCurrentWindows())
                {
                    Window::SetActive(w);
                    bool hasNotClosed = w->HandleEvent(sdlEvent);
                    if (!hasNotClosed) { windowsToBeClosed.PushBack(w); }
                    Window::SetActive(nullptr);
                }

                for (Window *w : windowsToBeClosed)
                {
                    Window::SetActive(w);
                    w->OnClosed();
                    GetCurrentWindows().Remove(w);
                    delete w;
                    Window::SetActive(nullptr);
                }
            }
        }
    }

    for (Window *w : GetCurrentWindows())
    {
        Window::SetActive(w);
        w->OnHandleEventsFinished();
        Window::SetActive(nullptr);
    }
    return true;
}

void WindowManager::DestroyQueuedWindows()
{
    Window *latestWindow = Window::GetActive();
    for (Window *w : p_windowsToBeDestroyed)
    {
        Window::SetActive(w);
        GetCurrentWindows().Remove(w);
        delete w;
    }
    p_windowsToBeDestroyed.Clear();
    Window::SetActive(latestWindow);
}

List<Window*> &WindowManager::GetCurrentWindows()
{
    return m_windowsStack.top();
}
const List<Window*> &WindowManager::GetCurrentWindows() const
{
    return m_windowsStack.top();
}

WindowManager *WindowManager::GetInstance()
{
    return Application::GetInstance()->GetWindowManager();
}


