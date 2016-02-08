#include "VAO.h"

VAO::VAO() : vboIdCounter(0)
{
    glGenVertexArrays(1, &idgl);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &idgl);
}

int VAO::BindVBO(const VBO *vbo,
                  VBOMeaning vboMeaning,
                  GLint dataComponentsCount,
                  GLenum dataType,
                  GLboolean dataNormalized,
                  GLsizei dataStride,
                  GLuint dataOffset)
{
    if(dataComponentsCount == -1)
    {
        if(vboMeaning == VBOMeaning::Position ||
           vboMeaning == VBOMeaning::Normal)
        {
            dataComponentsCount = 3;
        }
        else
        {
            dataComponentsCount = 2;
        }
    }

    this->Bind();

    vbo->Bind();

    glEnableVertexAttribArray(vboIdCounter);
    glVertexAttribPointer(vboIdCounter,
                          dataComponentsCount, dataType,
                          dataNormalized, dataStride, (void*) dataOffset);

    vbo->UnBind();

    this->UnBind();

    vbos[vboMeaning] = VBOLocation(vbo, vboIdCounter);
    return vboIdCounter++;
}

void VAO::UnBindVBO(VBOMeaning meaning)
{
    UnBindVBO(GetVBOIdByMeaning(meaning));
}

void VAO::UnBindVBO(int vboid)
{
    this->Bind();
    glDisableVertexAttribArray(vboid);
    this->UnBind();

    for(auto it = vbos.begin(); it != vbos.end(); ++it)
    {
        if((*it).second.vboid == vboid) vbos.erase(it);
    }
}


void VAO::Bind() const
{
    PreBind(GL_VERTEX_ARRAY_BINDING);
    glBindVertexArray(idgl);
}

void VAO::UnBind() const
{
    glBindVertexArray(PreUnBind());
}

int VAO::GetVBOIdByMeaning(VAO::VBOMeaning meaning) const
{
    if(vbos.find(meaning) == vbos.end()) return -1;
    else return vbos[meaning].vboid;
}
