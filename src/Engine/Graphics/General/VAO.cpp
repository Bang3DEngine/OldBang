#include "Bang/VAO.h"

#include "Bang/GL.h"
#include "Bang/IBO.h"
#include "Bang/VBO.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

VAO::VAO()
{
    GL::GenVertexArrays(1, &m_idGL);
}

VAO::~VAO()
{
    GL::DeleteVertexArrays(1, &m_idGL);
}

void VAO::AddVBO(const VBO *vbo,
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

    while (p_vbos.Size() <= location) p_vbos.PushBack(nullptr);
    p_vbos[location] = vbo;
}

void VAO::SetIBO(IBO *ebo)
{
    p_ibo = ebo;
}

void VAO::UnBindVBO(GLint location)
{
    if (location >= 0 && location < p_vbos.Size())
    {
        this->Bind();
        GL::DisableVertexAttribArray(location);
        this->UnBind();
        p_vbos[location] = nullptr;
    }
}

IBO *VAO::GetIBO() const
{
    return p_ibo;
}

GL::BindTarget VAO::GetGLBindTarget() const
{
    return GL::BindTarget::VAO;
}

void VAO::Bind() const
{
    GL::Bind(this);
    if (GetIBO()) { GetIBO()->Bind(); }
}
void VAO::UnBind() const
{
    if (GetIBO()) { GetIBO()->UnBind(); }
    GL::UnBind(this);
}

bool VAO::IsIndexed() const
{
    return (GetIBO() != nullptr);
}

const VBO* VAO::GetVBOByLocation(GLint location) const
{
    if (location >= p_vbos.Size()) return nullptr;
    else return p_vbos[location];
}

int VAO::GetVBOCount() const
{
    return p_vbos.Size();
}
