#include "Bang/G_Screen.h"

#include "Bang/GL.h"
#include "Bang/Debug.h"

G_Screen::G_Screen()
{
}

G_Screen::~G_Screen()
{
}

void G_Screen::Render()
{
}

void G_Screen::Initialize()
{
    glewExperimental = GL_TRUE;

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Debug_Error("Glew init error: " << glewGetErrorString(glewError));
    }

    GL::Enable(GL_DEPTH_TEST);
    GL::Enable(GL_STENCIL_TEST);
    GL::Enable(GL_CULL_FACE);
}

void G_Screen::OnResize(int w, int h)
{
    GL::SetViewport(0, 0, w, h);
    m_width = w;
    m_height = h;
}

float G_Screen::GetAspectRatio() const
{
    return float(GetWidth()) / GetHeight();
}

int G_Screen::GetWidth() const
{
    return m_width;
}

Vector2i G_Screen::GetSize() const
{
    return Vector2i(GetWidth(), GetHeight());
}

Vector2 G_Screen::GetPixelClipSize() const
{
    return 1.0f / Vector2(GetSize());
}

int G_Screen::GetHeight() const
{
    return m_height;
}

