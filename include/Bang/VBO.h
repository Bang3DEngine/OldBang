#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

class VBO : public GLObject
{
public:
    VBO();
    virtual ~VBO();

    void Fill(const void *data, int dataSize,
              GL_UsageHint usage = GL_UsageHint::StaticDraw);

    GL_BindTarget GetGLBindTarget() const override;
    void Bind() const override;
    void UnBind() const override;
};

NAMESPACE_BANG_END

#endif // VBO_H
