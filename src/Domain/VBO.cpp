#include "VBO.h"

VBO::VBO()
{
    glGenBuffers(1, &idgl);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &idgl);
}

void VBO::Fill(void *data, int dataSize, GLenum usage)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
    UnBind();
}

void VBO::Bind() const
{
    PreBind(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, idgl);
}
void VBO::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, PreUnBind());
}
