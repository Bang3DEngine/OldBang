#ifndef GLIDABLE_H
#define GLIDABLE_H

class GLIdable
{
protected:
    GLIdable() {}
    GLuint idgl;

public:
    GLuint GetGLId() { return idgl; }
};

#endif // GLIDABLE_H
