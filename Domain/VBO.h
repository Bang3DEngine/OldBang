#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "Idable.h"

class VBO : public Idable
{
private:
    mutable GLint lastBoundVBOId;

public:
    VBO();
    ~VBO();

    void Fill(void *data, int dataSize, GLenum usage = GL_STATIC_DRAW);

    void Bind() const;
    void UnBind() const;
};

#endif // VBO_H
