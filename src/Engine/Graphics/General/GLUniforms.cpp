#include "Bang/GLUniforms.h"

#include "Bang/UniformBuffer.h"

USING_NAMESPACE_BANG

IUniformBuffer::IUniformBuffer()
{
}

IUniformBuffer::~IUniformBuffer()
{
}

void IUniformBuffer::SetBindingPoint(int bindingPoint)
{
    m_bindingPoint = bindingPoint;
    Bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, GetGLId());
    UnBind();
}

GLuint IUniformBuffer::GetBindingPoint() const { return m_bindingPoint; }



void GLUniforms::RemoveBuffer(IUniformBuffer *buffer)
{
    if (m_uniformBuffers.ContainsValue(buffer))
    {
        delete buffer;
        m_uniformBuffers.RemoveValues(buffer);
    }
}

void GLUniforms::BindAllUniformBuffersToShader(const ShaderProgram *sp)
{
}

UniformBuffer<GLUniforms::Matrices> *GLUniforms::GetMatricesBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Matrices>();
}

GLUniforms::GLUniforms()
{
    UniformBuffer<GLUniforms::Matrices> *matricesBuffer =
                                    _CreateBuffer<GLUniforms::Matrices>();
}

GLUniforms::~GLUniforms()
{
    while (!m_uniformBuffers.IsEmpty())
    {
        RemoveBuffer(m_uniformBuffers.Begin()->second);
    }
}

GLUniforms *GLUniforms::GetActive()
{
    return GL::GetActive()->GetGLUniforms();
}

