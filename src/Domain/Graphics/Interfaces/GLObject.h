#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <stack>
#include <GL/glew.h>

#include "Bang/GL.h"
#include "Bang/Map.h"

class GLObject
{
public:
    GLId GetGLId() const;

    virtual GL::BindTarget GetGLBindTarget() const;
    virtual void Bind() const;
    virtual void UnBind() const;

protected:
    GLObject();

    GLId m_idGL = 0;
};

#endif // GLOBJECT_H
