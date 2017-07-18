#include "Bang/Screen.h"

#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/GraphicPipeline.h"

Screen::Screen()
{
}

Screen::~Screen()
{
    if (m_gPipeline) { delete m_gPipeline; }
}

void Screen::Initialize()
{
    Screen::Initialize();
    m_gPipeline = new GraphicPipeline(this);
}

void Screen::Render() const
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_gPipeline->RenderScene(activeScene, true);
    }
}

void Screen::Resize(int w, int h)
{
    Screen::Resize(w,h);
    ENSURE(m_gPipeline);

    m_gPipeline->OnResize(w, h);
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->_OnResize(w,h);
    }
}

float Screen::GetAspectRatio()
{
    G_Screen *screen = static_cast<G_Screen*>(Screen::GetInstance());
    return screen->GetAspectRatio();
}

int Screen::GetHeight()
{
    G_Screen *screen = static_cast<G_Screen*>(Screen::GetInstance());
    return screen->GetHeight();
}

int Screen::GetWidth()
{
    G_Screen *screen = static_cast<G_Screen*>(Screen::GetInstance());
    return screen->GetWidth();
}

Vector2 Screen::GetSize()
{
    G_Screen *screen = static_cast<G_Screen*>(Screen::GetInstance());
    return screen->GetSize();
}

Vector2 Screen::GetPixelClipSize()
{
    G_Screen *screen = static_cast<G_Screen*>(Screen::GetInstance());
    return screen->GetPixelClipSize();
}

Screen *Screen::GetInstance()
{
    Window *win = Application::GetInstance()->GetMainWindow();
    return win ? win->GetScreen() : nullptr;
}

GraphicPipeline *Screen::GetGraphicPipeline() const
{
    return m_gPipeline;
}
