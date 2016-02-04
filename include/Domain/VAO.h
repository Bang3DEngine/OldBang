#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "VBO.h"

class VAO : public GLIdable, GLBindable
{
private:
    int vboCount;

public:
    VAO();
    virtual ~VAO();

    int BindVBO(const VBO *vbo,
                GLuint dataComponentsCount,
                GLenum dataType            = GL_FLOAT,
                GLboolean dataNormalized   = GL_FALSE,
                GLsizei dataStride         = 0,
                const GLvoid* dataOffset   = (void*)0);

    void UnBindVBO(int index);

    void Bind() const override;
    void UnBind() const override;

    int GetVBOCount() const { return vboCount; }
};

#endif // VAO_H
