#ifndef IUNIFORMBUFFER_H
#define IUNIFORMBUFFER_H

#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

class IUniformBuffer : public GLObject
{
public:
    virtual ~IUniformBuffer();

    void SetBindingPoint(int bindingPoint);
    GLuint GetBindingPoint() const;

    GL::BindTarget GetGLBindTarget() const override;

protected:
    IUniformBuffer();

private:
    GLuint m_bindingPoint = -1;
};

NAMESPACE_BANG_END

#endif // IUNIFORMBUFFER_H

