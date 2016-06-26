#ifndef MESH_H
#define MESH_H

#include "Bang.h"

#include <GL/glew.h>
#include <vector>

#include "glm/glm.hpp"

#include "ShaderContract.h"
#include "IFileable.h"
#include "FileReader.h"
#include "Sphere.h"
#include "Asset.h"
#include "VAO.h"
#include "VBO.h"
#include "Box.h"

class Mesh : public Asset
{
friend class MeshRenderer;

public:
    const static std::string GetFileExtensionStatic() { return "bmesh"; }
    const virtual std::string GetFileExtension()
    {
        return Mesh::GetFileExtensionStatic();
    }

private:
    Box bBox;
    Sphere bSphere;

    VAO *vao = nullptr;
    VBO *vertexPositionsVBO = nullptr;
    VBO *vertexNormalsVBO = nullptr;
    VBO *vertexUvsVBO = nullptr;
    int vertexCount = 0;

    bool trisModel = false;

public:
    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadFromFile(const std::string &filepath);
    void LoadPositions(const std::vector<Vector3>& positions);
    void LoadNormals(const std::vector<Vector3>& normals);
    void LoadUvs(const std::vector<glm::vec2>& uvs);
    void LoadAll(const std::vector<Vector3>& positions,
                 const std::vector<Vector3>& normals,
                 const std::vector<glm::vec2>& uvs);

    void BindPositionsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);
    void BindNormalsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);
    void BindUvsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp);

    void BindAllVBOsToShaderProgram(const ShaderProgram &sp);

    VAO *GetVAO() const;
    int GetVertexCount() const;
    bool IsATrianglesModel() const;
    const Box& GetBoundingBox() const;
    const Sphere& GetBoundingSphere() const;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // MESH_H
