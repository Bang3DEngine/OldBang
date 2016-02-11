#ifndef GLIDABLE_H
#define GLIDABLE_H

#include <GL/glew.h>

class GLIdable
{
protected:
    GLIdable() {}
    GLuint idgl;

public:
    GLuint GetGLId() { return idgl; }
};

#endif // GLIDABLE_H
