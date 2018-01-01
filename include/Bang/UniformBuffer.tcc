#include "Bang/UniformBuffer.h"

USING_NAMESPACE_BANG

template<class BufferStruct>
UniformBuffer<BufferStruct>::UniformBuffer()
{
    GL::GenBuffers(1, &m_idGL);

    Bind();
    GL::BufferData(GL_BindTarget::UniformBuffer,
                   sizeof(BufferStruct), &m_data,
                   GL_UsageHint::DynamicDraw);
    UnBind();
}

template<class BufferStruct>
UniformBuffer<BufferStruct>::~UniformBuffer()
{
    GL::DeleteBuffers(1, &m_idGL);
}

template<class BufferStruct>
void UniformBuffer<BufferStruct>::Set(const BufferStruct &data)
{
    m_data = data;
    UpdateBuffer();
}

template<class BufferStruct>
BufferStruct* const UniformBuffer<BufferStruct>::GetData()
{
    return &m_data;
}
template<class BufferStruct>
const BufferStruct* const UniformBuffer<BufferStruct>::GetData() const
{
    return &m_data;
}

template<class BufferStruct>
void UniformBuffer<BufferStruct>::UpdateBuffer() const
{
    Bind();

    GLvoid* p = GL::MapBuffer( GetGLBindTarget(), GL_Enum::WriteOnly );
    memcpy(p, GetData(), sizeof(BufferStruct));
    GL::UnMapBuffer( GetGLBindTarget() );

    UnBind();
}

template<class BufferStruct>
GL_BindTarget UniformBuffer<BufferStruct>::GetGLBindTarget() const
{
    return GL_BindTarget::UniformBuffer;
}

