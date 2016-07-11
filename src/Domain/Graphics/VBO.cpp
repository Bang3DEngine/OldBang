#include "VBO.h"

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

void VBO::Bind() const
{
    PreBind(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_idGL);
}
void VBO::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, PreUnBind(GL_ARRAY_BUFFER));
}
