#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

class IUniformBuffer : public GLObject
{
public:
    virtual ~IUniformBuffer();

    void SetBindingPoint(int bindingPoint);
    GLuint GetBindingPoint() const;

protected:
    IUniformBuffer();

private:
    GLuint m_bindingPoint;
};


template<class BufferStruct>
class UniformBuffer : public IUniformBuffer
{
public:
	UniformBuffer();
	virtual ~UniformBuffer();

    void Set(const BufferStruct &data);
    BufferStruct* const GetData();
    const BufferStruct* const GetData() const;

    void UpdateBuffer() const;

    virtual GL::BindTarget GetGLBindTarget() const;

private:
    BufferStruct m_data;

    friend class GLUniforms;
};

NAMESPACE_BANG_END

#include "Bang/UniformBuffer.tcc"

#endif // UNIFORMBUFFER_H

