#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include "IGLIdable.h"
#include "IGLBindable.h"

class VBO : public IGLIdable, public IGLBindable
{
public:
    VBO();
    virtual ~VBO();

    void Fill(void *data, int dataSize, GLenum usage = GL_STATIC_DRAW);

    void Bind() const override;
    void UnBind() const override;
};

#endif // VBO_H
