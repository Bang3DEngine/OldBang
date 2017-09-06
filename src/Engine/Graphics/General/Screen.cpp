#include "Bang/Screen.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/GraphicPipeline.h"

USING_NAMESPACE_BANG

Screen::Screen()
{
}

Screen::~Screen()
{
    if (m_gPipeline) { delete m_gPipeline; }
}

void Screen::Initialize()
{
    glewExperimental = true;

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Debug_Error("Glew init error: " << glewGetErrorString(glewError));
    }

    GL::Enable(GL::Test::Depth);
    GL::Enable(GL::Test::Stencil);
    GL::Enable(GL::Test::CullFace);

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

void Screen::OnResize(int w, int h)
{
    GL::SetViewport(0, 0, w, h);
    m_size.x = w;
    m_size.y = h;

    ENSURE(m_gPipeline);

    m_gPipeline->OnResize(w, h);
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->_OnResize(w,h);
    }
}

float Screen::GetAspectRatio() const
{
    return float(GetWidth()) / GetHeight();
}

int Screen::GetWidth() const
{
    return m_size.x;
}

const Vector2i& Screen::GetSize() const
{
    return m_size;
}

Vector2 Screen::GetPixelClipSize() const
{
    return 1.0f / Vector2(GetSize());
}

int Screen::GetHeight() const
{
    return m_size.y;
}

float Screen::GetAspectRatioS()
{
    return Screen::GetInstance()->GetAspectRatio();
}

int Screen::GetHeightS()
{
    return Screen::GetInstance()->GetHeight();
}

int Screen::GetWidthS()
{
    return Screen::GetInstance()->GetWidth();
}

const Vector2i& Screen::GetSizeS()
{
    return Screen::GetInstance()->GetSize();
}

Vector2 Screen::GetPixelClipSizeS()
{
    return Screen::GetInstance()->GetPixelClipSize();
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
