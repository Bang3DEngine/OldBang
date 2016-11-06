#ifndef MESH_H
#define MESH_H

#include "Bang.h"

#include <GL/glew.h>
#include "Array.h"

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
public:
    const static String GetFileExtensionStatic() { return "bmesh"; }
    const virtual String GetFileExtension()
    {
        return Mesh::GetFileExtensionStatic();
    }

private:
    Box m_bBox;
    Sphere m_bSphere;

    Array<Vector3> m_positions;
    Array<Vector3> m_normals;
    Array<Vector2> m_uvs;

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

    void LoadFromFile(const String &m_filepath);
    void LoadPositions(const Array<Vector3>& positions);
    void LoadNormals(const Array<Vector3>& normals);
    void LoadUvs(const Array<Vector2>& uvs);
    void LoadAll(const Array<Vector3>& positions,
                 const Array<Vector3>& normals,
                 const Array<Vector2>& uvs);

    void BindPositionsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);
    void BindNormalsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);
    void BindUvsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);

    void BindAllVBOsToShaderProgram(const ShaderProgram &sp);

    VAO *GetVAO() const;
    int GetVertexCount() const;
    bool IsATrianglesModel() const;
    const Box& GetBoundingBox() const;
    const Sphere& GetBoundingSphere() const;

    const Array<Vector3>& GetPositions();
    const Array<Vector3>& GetNormals();
    const Array<Vector2>& GetUvs();

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    friend class MeshRenderer;
};

#endif // MESH_H
