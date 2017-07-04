#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/GLObject.h"

class G_VBO;
class G_VAO : public GLObject
{
public:
    G_VAO();
    virtual ~G_VAO();

    void BindVBO(const G_VBO *vbo,
                 GLint location,
                 GLint dataComponentsCount = -1,
                 GLenum dataType           = GL_FLOAT,
                 GLboolean dataNormalized  = GL_FALSE,
                 GLsizei dataStride        = 0,
                 GLuint dataOffset         = 0);

    void UnBindVBO(GLint location);

    GL::BindTarget GetGLBindTarget() const override;

    void Bind() const override;
    void UnBind() const override;

    const G_VBO *GetVBOByLocation(int location) const;

    int GetVBOCount() const;

private:
    Array<const G_VBO*> m_vbos;
};

#endif // VAO_H
