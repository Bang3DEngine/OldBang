#include "VAO.h"

VAO::VAO() : lastBoundVAOId(0), vboCount(0)
{
    glGenVertexArrays(1, &idgl);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &idgl);
}

void VAO::BindVBO(const VBO& vbo,
                  GLuint dataComponentsCount,
                  GLenum dataType,
                  GLboolean dataNormalized,
                  GLsizei dataStride,
                  const GLvoid* dataOffset)
{
    this->Bind();

    vbo.Bind();

    glEnableVertexAttribArray(vboCount);
    glVertexAttribPointer(vboCount,
                          dataComponentsCount, dataType,
                          dataNormalized, dataStride, dataOffset);

    vbo.UnBind();

    this->UnBind();

    ++vboCount;
}

void VAO::Bind() const
{
    if(lastBoundVAOId == -1) glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastBoundVAOId);
    glBindVertexArray(idgl);
}

void VAO::UnBind() const
{
    if(lastBoundVAOId != -1) glBindVertexArray(lastBoundVAOId);
    lastBoundVAOId = -1;
}
