#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &idgl);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &idgl);
}

void VAO::BindVBO(const VBO *vbo,
                  GLint location,
                  GLint dataComponentsCount,
                  GLenum dataType,
                  GLboolean dataNormalized,
                  GLsizei dataStride,
                  GLuint dataOffset)
{
    UnBindVBO(location); //unbind in case its a vbo replacement

    if(location < 0) return;

    this->Bind();

    vbo->Bind();

    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location,
                          dataComponentsCount, dataType,
                          dataNormalized, dataStride, (void*) dataOffset);

    vbo->UnBind();

    this->UnBind();

    while(vbos.size() <= location) vbos.push_back(nullptr);
    vbos[location] = vbo;
}

void VAO::UnBindVBO(GLint location)
{
    if(location >= 0 && location < vbos.size())
    {
        this->Bind();
        glDisableVertexAttribArray(location);
        this->UnBind();
        vbos[location] = nullptr;
    }
}


void VAO::Bind() const
{
    PreBind(GL_VERTEX_ARRAY_BINDING);
    glBindVertexArray(idgl);
}

void VAO::UnBind() const
{
    glBindVertexArray(PreUnBind(GL_VERTEX_ARRAY_BINDING));
}

const VBO* VAO::GetVBOByLocation(GLint location) const
{
    if(location >= vbos.size()) return nullptr;
    else return vbos[location];
}
