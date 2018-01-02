#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "Bang/GLEnums.h"

NAMESPACE_BANG_BEGIN

class GLObject
{
public:
    GLId GetGLId() const;

    virtual GL_BindTarget GetGLBindTarget() const;
    virtual void Bind() const;
    virtual void UnBind() const;

protected:
    GLObject();

    GLId m_idGL = 0;
};

NAMESPACE_BANG_END

#endif // GLOBJECT_H
