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
    if(lastBoundId == -1) glGetIntegerv(GL_ARRAY_BUFFER, &lastBoundId);
    glBindBuffer(GL_ARRAY_BUFFER, idgl);
}

void VBO::UnBind() const
{
    //needed if, if UnBind called multiple times
    if(lastBoundId != -1) glBindBuffer(GL_ARRAY_BUFFER, lastBoundId);
    lastBoundId = -1;
}
