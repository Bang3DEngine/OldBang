#ifndef MESH_H
#define MESH_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Sphere.h"

class VAO;
class VBO;
class Sphere;
class Vector2;
class Vector3;
class XMLNode;
class FileReader;
class SerializableObject;
class Mesh : public Asset
{
	OBJECT(Mesh)
    ASSET_ICON(Mesh, "Icons/MeshAssetIcon.png")

public:
    static constexpr uint DefaultPositionsVBOLocation = 0;
    static constexpr uint DefaultNormalsVBOLocation   = 1;
    static constexpr uint DefaultUvsVBOLocation       = 2;

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

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

    VAO *GetVAO() const;
    int GetVertexCount() const;
    bool IsATrianglesModel() const;
    const AABox& GetAABBox() const;
    const Sphere& GetBoundingSphere() const;

    const Array<Vector3>& GetPositions();
    const Array<Vector3>& GetNormals();
    const Array<Vector2>& GetUvs();

    const String &GetMeshFilepath() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void BindPositionsVBOToLocation(int positionsVBOLocation);
    void BindNormalsVBOToLocation(int normalsVBOLocation);
    void BindUvsVBOToLocation(int uvsVBOLocation);

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
