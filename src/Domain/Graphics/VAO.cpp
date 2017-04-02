#include "Bang/VAO.h"

#include "Bang/VBO.h"
#include "Bang/ShaderProgram.h"

VAO::VAO()
{
    glGenVertexArrays(1, &m_idGL);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_idGL);
}

void VAO::BindVBO(const VBO *vbo,
                  GLint location,
                  GLint dataComponentsCount,
                  GLenum dataType,
                  GLboolean dataNormalized,
                  GLsizei dataStride,
                  GLuint dataOffset)
{
    UnBindVBO(location); // unbind in case its a vbo replacement

    if (location < 0) return;

    Bind();

    vbo->Bind();
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location,
                          dataComponentsCount, dataType,
                          dataNormalized, dataStride, (void*) dataOffset);
    vbo->UnBind();

    UnBind();

    while (m_vbos.Size() <= location) m_vbos.PushBack(nullptr);
    m_vbos[location] = vbo;
}

void VAO::UnBindVBO(GLint location)
{
    if (location >= 0 && location < m_vbos.Size())
    {
        this->Bind();
        glDisableVertexAttribArray(location);
        this->UnBind();
        m_vbos[location] = nullptr;
    }
}

GL::BindTarget VAO::GetGLBindTarget() const
{
    return GL::BindTarget::VAO;
}

void VAO::Bind() const
{
    GL::Bind(this);
}
void VAO::UnBind() const
{
    GL::UnBind(this);
}

const VBO* VAO::GetVBOByLocation(GLint location) const
{
    if (location >= m_vbos.Size()) return nullptr;
    else return m_vbos[location];
}

int VAO::GetVBOCount() const
{
    return m_vbos.Size();
}
