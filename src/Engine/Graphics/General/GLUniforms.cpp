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

UniformBuffer<GLUniforms::Matrices> *GLUniforms::GetMatricesBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Matrices>();
}

UniformBuffer<GLUniforms::Camera> *GLUniforms::GetCameraBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Camera>();
}

UniformBuffer<GLUniforms::Viewport> *GLUniforms::GetViewportBuffer()
{
    return GLUniforms::GetBuffer<GLUniforms::Viewport>();
}

GLUniforms::GLUniforms()
{
    _CreateBuffer<GLUniforms::Matrices>();
    _CreateBuffer<GLUniforms::Camera>();
    _CreateBuffer<GLUniforms::Viewport>();
}

void GLUniforms::BindAllUniformBuffersToShader(const ShaderProgram *sp)
{
    GL::BindUniformBufferToShader("MatricesBlock", sp, GetMatricesBuffer());
    GL::BindUniformBufferToShader("CameraBlock", sp, GetCameraBuffer());
    GL::BindUniformBufferToShader("ViewportBlock", sp, GetViewportBuffer());
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

