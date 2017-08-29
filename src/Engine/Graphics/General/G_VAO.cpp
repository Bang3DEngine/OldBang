#include "Bang/G_VAO.h"

#include "Bang/G_VBO.h"
#include "Bang/G_ShaderProgram.h"

G_VAO::G_VAO()
{
    GL::GenVertexArrays(1, &m_idGL);
}

G_VAO::~G_VAO()
{
    GL::DeleteVertexArrays(1, &m_idGL);
}

void G_VAO::BindVBO(const G_VBO *vbo,
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

void G_VAO::UnBindVBO(GLint location)
{
    if (location >= 0 && location < m_vbos.Size())
    {
        this->Bind();
        GL::DisableVertexAttribArray(location);
        this->UnBind();
        m_vbos[location] = nullptr;
    }
}

GL::BindTarget G_VAO::GetGLBindTarget() const
{
    return GL::BindTarget::VAO;
}

void G_VAO::Bind() const
{
    GL::Bind(this);
}
void G_VAO::UnBind() const
{
    GL::UnBind(this);
}

const G_VBO* G_VAO::GetVBOByLocation(GLint location) const
{
    if (location >= m_vbos.Size()) return nullptr;
    else return m_vbos[location];
}

int G_VAO::GetVBOCount() const
{
    return m_vbos.Size();
}
