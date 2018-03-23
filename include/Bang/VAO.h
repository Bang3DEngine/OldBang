#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class VBO;
FORWARD class IBO;

class VAO : public GLObject
{
public:
    VAO();
    virtual ~VAO();

    void Bind() const override;
    void BindVBO(const VBO *vbo,
                 int location,
                 int dataComponentsCount,
                 GL::DataType dataType = GL::DataType::Float,
                 bool dataNormalized   = false,
                 int dataStride        = 0,
                 int dataOffset        = 0);
    void BindIBO(IBO *ibo);

    void UnBind() const override;
    void UnBindVBO(GLint location);

    IBO *GetIBO() const;
    bool IsIndexed() const;
    int GetVBOCount() const;
    GL::BindTarget GetGLBindTarget() const override;
    const VBO *GetVBOByLocation(int location) const;

private:
    Array<const VBO*> p_vbos;
    IBO* p_ibo = nullptr;
};

NAMESPACE_BANG_END

#endif // VAO_H
