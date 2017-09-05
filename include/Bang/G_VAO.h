#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/GLObject.h"

FORWARD class G_VBO;

class G_VAO : public GLObject
{
public:
    G_VAO();
    virtual ~G_VAO();

    void BindVBO(const G_VBO *vbo,
                 int location,
                 int dataComponentsCount,
                 GL::DataType dataType = GL::DataType::Float,
                 bool dataNormalized   = false,
                 int dataStride        = 0,
                 int dataOffset        = 0);

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