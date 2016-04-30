#ifndef MESH_H
#define MESH_H

#include "Bang.h"

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "ShaderContract.h"
#include "IFileable.h"
#include "FileReader.h"
#include "Asset.h"
#include "VAO.h"
#include "VBO.h"

#include <vector>

class Mesh : public Asset
{
friend class MeshRenderer;

public:
    const static std::string GetFileExtensionStatic() { return "bmesh"; }
    const virtual std::string GetFileExtension()
    {
        return Mesh::GetFileExtensionStatic();
    }

    enum RenderMode
    {
        Points = GL_POINTS,
        Lines = GL_LINES,
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

private:
    VAO *vao;
    VBO *vertexPositionsVBO;
    VBO *vertexNormalsVBO;
    VBO *vertexUvsVBO;
    int vertexCount;

protected:
    Mesh::RenderMode renderMode;

public:
    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadFromFile(const std::string &filepath);
    void LoadPositions(const std::vector<glm::vec3>& positions);
    void LoadNormals(const std::vector<glm::vec3>& normals);
    void LoadUvs(const std::vector<glm::vec2>& uvs);

    void BindPositionsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);
    void BindNormalsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);
    void BindUvsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);

    void BindAllVBOsToShaderProgram(const ShaderProgram &sp);

    void SetRenderMode(RenderMode renderMode);

    VAO *GetVAO() const;
    RenderMode GetRenderMode() const;
    int GetVertexCount() const;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // MESH_H
