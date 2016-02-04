#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "GLIdable.h"
#include "GLBindable.h"

class VBO : public GLIdable, public GLBindable
{
public:
    VBO();
    ~VBO();

    void Fill(void *data, int dataSize, GLenum usage = GL_STATIC_DRAW);

    void Bind() const override;
    void UnBind() const override;
};

#endif // VBO_H
