#include "Bang/IUniformBuffer.h"

USING_NAMESPACE_BANG

IUniformBuffer::IUniformBuffer()
{

}

IUniformBuffer::~IUniformBuffer()
{
}

void IUniformBuffer::SetBindingPoint(int bindingPoint)
{
    if (bindingPoint != GetBindingPoint())
    {
        m_bindingPoint = bindingPoint;
        Bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, GetGLId());
        UnBind();
    }
}

GLuint IUniformBuffer::GetBindingPoint() const { return m_bindingPoint; }

GL::BindTarget IUniformBuffer::GetGLBindTarget() const
{
    return GL::BindTarget::UniformBuffer;
}
