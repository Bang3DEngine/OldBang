#include "Bang/GLUniforms.h"

USING_NAMESPACE_BANG

template<class T>
UniformBuffer<T> *GLUniforms::_CreateBuffer()
{
    if (!m_uniformBuffers.ContainsKey<T>())
    {
        UniformBuffer<T> *buffer = new UniformBuffer<T>();
        buffer->SetBindingPoint(m_bindingPointsUsed);
        m_uniformBuffers.Add<T>(buffer);
        ++m_bindingPointsUsed;
        return buffer;
    }
    return nullptr;
}

template <class T>
UniformBuffer<T>* GLUniforms::_GetBuffer()
{
    IUniformBuffer *iBuffer = m_uniformBuffers.Get<T>();
    if (iBuffer)
    {
        UniformBuffer<T> *buffer = Cast< UniformBuffer<T>* >(iBuffer);
        return buffer;
    }
    return nullptr;
}

template <class T>
void GLUniforms::_RemoveBuffer()
{
    IUniformBuffer *uniformBuffer = GetBuffer<T>();
    if (uniformBuffer) { RemoveBuffer(uniformBuffer); }
}


template <class T>
UniformBuffer<T>* GLUniforms::CreateBuffer()
{ return GLUniforms::GetActive()->_CreateBuffer<T>(); }

template <class T>
UniformBuffer<T>* GLUniforms::GetBuffer()
{ return GLUniforms::GetActive()->_GetBuffer<T>(); }

template <class T>
void GLUniforms::RemoveBuffer()
{ GLUniforms::GetActive()->_RemoveBuffer<T>(); }


