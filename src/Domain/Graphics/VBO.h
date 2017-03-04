#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "GLObject.h"

class VBO : public GLObject
{
public:
    VBO();
    virtual ~VBO();

    void Fill(const void *data, int dataSize, GLenum usage = GL_STATIC_DRAW);

    GL::BindTarget GetGLBindTarget() const override;
    void Bind() const override;
    void UnBind() const override;
};

#endif // VBO_H
