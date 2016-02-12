#ifndef IGLIDABLE_H
#define IGLIDABLE_H

#include <GL/glew.h>

class IGLIdable
{
protected:
    IGLIdable() {}
    GLuint idgl;

public:
    GLuint GetGLId() { return idgl; }
};

#endif // GLIDABLE_H
