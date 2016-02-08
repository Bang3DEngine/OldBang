#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include <map>

#include "VBO.h"

class VAO : public GLIdable, GLBindable
{
public:
    enum VBOMeaning
    {
        Position,
        Normal,
        UV,
        Other_0, Other_1, Other_2, Other_3,
        Other_4, Other_5, Other_6, Other_7
    };

private:

    struct VBOLocation
    {
        const VBO *vbo;
        int vboid;
        VBOLocation() : vbo(nullptr), vboid(-1) {}
        VBOLocation(const VBO *vbo, int id) : vbo(vbo), vboid(id) {}
    };

    int vboIdCounter;
    mutable std::map<VBOMeaning, VBOLocation> vbos;

public:

    VAO();
    virtual ~VAO();

    int BindVBO(const VBO *vbo,
                VBOMeaning vboMeaning,
                GLint dataComponentsCount = -1,
                GLenum dataType           = GL_FLOAT,
                GLboolean dataNormalized  = GL_FALSE,
                GLsizei dataStride        = 0,
                GLuint dataOffset         = 0);

    void UnBindVBO(VBOMeaning meaning);
    void UnBindVBO(int vboid);

    void Bind() const override;
    void UnBind() const override;

    int GetVBOIdByMeaning(VBOMeaning meaning) const;

    int GetVBOCount() const { return vbos.size(); }
};

#endif // VAO_H
