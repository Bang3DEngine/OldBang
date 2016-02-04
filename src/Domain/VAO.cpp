#include "VAO.h"

VAO::VAO() : vboCount(0)
{
    glGenVertexArrays(1, &idgl);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &idgl);
}

int VAO::BindVBO(const VBO *vbo,
                  GLuint dataComponentsCount,
                  GLenum dataType,
                  GLboolean dataNormalized,
                  GLsizei dataStride,
                  const GLvoid* dataOffset)
{
    this->Bind();

    vbo->Bind();

    glEnableVertexAttribArray(vboCount);
    glVertexAttribPointer(vboCount,
                          dataComponentsCount, dataType,
                          dataNormalized, dataStride, dataOffset);

    vbo->UnBind();

    this->UnBind();

    return vboCount++;
}

void VAO::UnBindVBO(int index)
{
    this->Bind();
    glDisableVertexAttribArray(index);
    this->UnBind();

    --vboCount;
}


void VAO::Bind() const
{
    PreBind(GL_VERTEX_ARRAY_BINDING);
    glBindVertexArray(idgl);
}

void VAO::UnBind() const
{
    glBindVertexArray(PreUnBind());
}
