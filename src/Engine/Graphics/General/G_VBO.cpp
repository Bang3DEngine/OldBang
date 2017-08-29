#include "Bang/G_VBO.h"

G_VBO::G_VBO()
{
    GL::GenBuffers(1, &m_idGL);
}

G_VBO::~G_VBO()
{
    GL::DeleteBuffers(1, &m_idGL);
}

void G_VBO::Fill(const void *data, int dataSize, GL::UsageHint usage)
{
    Bind();
    GL::BufferDataVBO(dataSize, data, usage);
    UnBind();
}

GL::BindTarget G_VBO::GetGLBindTarget() const
{
    return GL::BindTarget::VBO;
}

void G_VBO::Bind() const
{
    GL::Bind(this);
}
void G_VBO::UnBind() const
{
    GL::UnBind(this);
}
