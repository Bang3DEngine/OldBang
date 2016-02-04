#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "VBO.h"
#include "Idable.h"

class VAO : public GLIdable
{
private:
    mutable GLint lastBoundVAOId;
    int vboCount;

public:
    VAO();
    ~VAO();

    void BindVBO(const VBO& vbo,
                 GLuint dataComponentsCount,
                 GLenum dataType            = GL_FLOAT,
                 GLboolean dataNormalized   = GL_FALSE,
                 GLsizei dataStride         = 0,
                 const GLvoid* dataOffset   = (void*)0);

    void Bind() const;
    void UnBind() const;
};

#endif // VAO_H
