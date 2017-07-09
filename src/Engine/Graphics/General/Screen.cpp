#include "Bang/Screen.h"

#include "Bang/Scene.h"
#include "Bang/IWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/GraphicPipeline.h"

Screen::Screen(QWidget *parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
    setAutoBufferSwap(false);
}

Screen::~Screen()
{
    if (m_gPipeline) { delete m_gPipeline; }
}

void Screen::Initialize()
{
    static bool firstInitializeGL = true;
    if (firstInitializeGL)
    {
        firstInitializeGL = false;

        OnInitialize();
        m_gPipeline = new GraphicPipeline(this);
    }
}

void Screen::initializeGL()
{
    Initialize();
}

void Screen::paintGL()
{
    Render();
}

void Screen::Render() const
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_gPipeline->RenderScene(activeScene, true);
    }
}

void Screen::resizeGL(int w, int h)
{
    OnResize(w,h);
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
    IWindow *win = IWindow::GetInstance();
    return win ? win->GetScreen() : nullptr;
}

GraphicPipeline *Screen::GetGraphicPipeline() const
{
    return m_gPipeline;
}
