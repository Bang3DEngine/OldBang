#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>
#include <map>

#include "ShaderProgram.h"
#include "VBO.h"

class VAO : public IGLIdable, IGLBindable
{
private:

    std::vector<const VBO*> vbos;

public:

    VAO();
    virtual ~VAO();

    void BindVBO(const VBO *vbo,
                 GLint location,
                 GLint dataComponentsCount = -1,
                 GLenum dataType           = GL_FLOAT,
                 GLboolean dataNormalized  = GL_FALSE,
                 GLsizei dataStride        = 0,
                 GLuint dataOffset         = 0);

    void UnBindVBO(GLint location);

    void Bind() const override;
    void UnBind() const override;

    const VBO *GetVBOByLocation(int location) const;

    int GetVBOCount() const { return vbos.size(); }
};

#endif // VAO_H
