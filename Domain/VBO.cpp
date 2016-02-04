#include "VBO.h"

VBO::VBO() : lastBoundVBOId(-1)
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
    if(lastBoundVBOId == -1) glGetIntegerv(GL_ARRAY_BUFFER, &lastBoundVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, idgl);
}

void VBO::UnBind() const
{
    if(lastBoundVBOId != -1) glBindBuffer(GL_ARRAY_BUFFER, lastBoundVBOId);
    lastBoundVBOId = -1;
}
