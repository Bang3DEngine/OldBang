#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class VBO;

class VAO : public GLObject
{
public:
    VAO();
    virtual ~VAO();

    void BindVBO(const VBO *vbo,
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

    const VBO *GetVBOByLocation(int location) const;

    int GetVBOCount() const;

private:
    Array<const VBO*> m_vbos;
};

NAMESPACE_BANG_END

#endif // VAO_H
