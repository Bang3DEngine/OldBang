#ifndef IGLIDABLE_H
#define IGLIDABLE_H

#include <GL/glew.h>

class IGLIdable
{
protected:
    IGLIdable() {}
    GLuint m_idGL;

public:
    GLuint GetGLId() const { return m_idGL; }
};

#endif // GLIDABLE_H
