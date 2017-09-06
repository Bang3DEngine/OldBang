#include "Bang/VAO.h"

#include "Bang/VBO.h"
#include "Bang/ShaderProgram.h"

VAO::VAO()
{
    GL::GenVertexArrays(1, &m_idGL);
}

VAO::~VAO()
{
    GL::DeleteVertexArrays(1, &m_idGL);
}

void VAO::BindVBO(const VBO *vbo,
                  int location,
                  int dataComponentsCount,
                  GL::DataType dataType,
                  bool dataNormalized,
                  int dataStride,
                  int dataOffset)
{
    UnBindVBO(location); // unbind in case its a vbo replacement

    if (location < 0) return;

    Bind();

    vbo->Bind();
    GL::EnableVertexAttribArray(location);
    GL::VertexAttribPointer(location,
                            dataComponentsCount, dataType,
                            dataNormalized, dataStride,
                            dataOffset);
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
        GL::DisableVertexAttribArray(location);
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
