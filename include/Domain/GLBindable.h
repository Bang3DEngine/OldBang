#ifndef GLBINDABLE_H
#define GLBINDABLE_H

#include <GL/glew.h>

class GLBindable
{
protected:
    mutable GLint lastBoundId;
    GLBindable() : lastBoundId(-1) {}

public:
    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

#endif // GLBINDABLE_H
