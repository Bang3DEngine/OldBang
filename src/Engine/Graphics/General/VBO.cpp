#include "Bang/VBO.h"

VBO::VBO()
{
    glGenBuffers(1, &m_idGL);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_idGL);
}

void VBO::Fill(const void *data, int dataSize, GLenum usage)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
    UnBind();
}

GL::BindTarget VBO::GetGLBindTarget() const
{
    return GL::BindTarget::VBO;
}

void VBO::Bind() const
{
    GL::Bind(this);
}
void VBO::UnBind() const
{
    GL::UnBind(this);
}
