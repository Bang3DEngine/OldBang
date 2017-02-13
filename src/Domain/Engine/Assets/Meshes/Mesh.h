#ifndef MESH_H
#define MESH_H

#include "AABox.h"
#include "Array.h"
#include "Asset.h"
#include "Sphere.h"

class VAO;
class VBO;
class Sphere;
class Vector2;
class Vector3;
class XMLNode;
class IFileable;
class FileReader;
class ShaderProgram;
class Mesh : public Asset
{
public:
    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadFromFile(const String &m_assetFilepath);
    void LoadPositions(const Array<Vector3>& positions);
    void LoadNormals(const Array<Vector3>& normals);
    void LoadUvs(const Array<Vector2>& uvs);
    void LoadAll(const Array<Vector3>& positions,
                 const Array<Vector3>& normals,
                 const Array<Vector2>& uvs);

    void BindPositionsToShaderProgram(const String &nameInShader,
                                      const ShaderProgram &sp) const;
    void BindNormalsToShaderProgram(const String &nameInShader,
                                    const ShaderProgram &sp) const;
    void BindUvsToShaderProgram(const String &nameInShader,
                                const ShaderProgram &sp) const;

    void BindAllVBOsToShaderProgram(const ShaderProgram &sp);

    VAO *GetVAO() const;
    int GetVertexCount() const;
    bool IsATrianglesModel() const;
    const AABox& GetAABBox() const;
    const Sphere& GetBoundingSphere() const;

    const Array<Vector3>& GetPositions();
    const Array<Vector3>& GetNormals();
    const Array<Vector2>& GetUvs();

    const String &GetMeshFilepath() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

private:
    String m_meshFilepath = "";

    Array<Vector3> m_positions;
    Array<Vector3> m_normals;
    Array<Vector2> m_uvs;

    mutable VAO *m_vao = nullptr;
    VBO *m_vertexPositionsVBO = nullptr;
    VBO *m_vertexNormalsVBO = nullptr;
    VBO *m_vertexUvsVBO = nullptr;
    int m_vertexCount = 0;

    AABox m_bBox;
    Sphere m_bSphere;

    bool m_trisModel = false;

    friend class MeshRenderer;
};

#endif // MESH_H
