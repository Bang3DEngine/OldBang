#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "Bang/GLObject.h"

class VBO : public GLObject
{
public:
    VBO();
    virtual ~VBO();

    void Fill(const void *data, int dataSize,
              GL::UsageHint usage = GL::UsageHint::StaticDraw);

    GL::BindTarget GetGLBindTarget() const override;
    void Bind() const override;
    void UnBind() const override;
};

#endif // VBO_H
