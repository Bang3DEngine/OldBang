#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "Bang/IUniformBuffer.h"

NAMESPACE_BANG_BEGIN

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

    virtual GL_BindTarget GetGLBindTarget() const;

private:
    BufferStruct m_data;

    friend class GLUniforms;
};

NAMESPACE_BANG_END

#include "Bang/UniformBuffer.tcc"

#endif // UNIFORMBUFFER_H

