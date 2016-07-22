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
    Box m_bBox;
    Sphere m_bSphere;

    VAO *m_vao = nullptr;
    VBO *m_vertexPositionsVBO = nullptr;
    VBO *m_vertexNormalsVBO = nullptr;
    VBO *m_vertexUvsVBO = nullptr;
    int m_vertexCount = 0;

    bool m_trisModel = false;

public:
    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadFromFile(const std::string &m_filepath);
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

    virtual std::string GetTag() const override;
    virtual void WriteInternal(std::ostream &f) const;
    virtual void ReadInternal(std::istream &f);
};

#endif // MESH_H
