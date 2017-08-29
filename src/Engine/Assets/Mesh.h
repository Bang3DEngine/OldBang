#ifndef MESH_H
#define MESH_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Sphere.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"

FORWARD class G_VAO;
FORWARD class G_VBO;

class Mesh : public Asset
{
    ASSET(Mesh)

public:
    static constexpr uint DefaultPositionsVBOLocation = 0;
    static constexpr uint DefaultNormalsVBOLocation   = 1;
    static constexpr uint DefaultUvsVBOLocation       = 2;

    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadPositions(const Array<Vector3>& positions);
    void LoadNormals(const Array<Vector3>& normals);
    void LoadUvs(const Array<Vector2>& uvs);
    void LoadAll(const Array<Vector3>& positions,
                 const Array<Vector3>& normals,
                 const Array<Vector2>& uvs);

    void BindPositionsVBOToLocation(int positionsVBOLocation);
    void BindNormalsVBOToLocation(int normalsVBOLocation);
    void BindUvsVBOToLocation(int uvsVBOLocation);

    G_VAO *GetVAO() const;
    int GetVertexCount() const;
    const AABox& GetAABBox() const;
    const Sphere& GetBoundingSphere() const;
    const Array<Vector3>& GetPositions();
    const Array<Vector3>& GetNormals();
    const Array<Vector2>& GetUvs();
    const Path &GetModelFilepath() const;

    // Resource
    void Import(const Path &meshFilepath) override;

    // SerializableObject
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Path m_modelFilepath;

    Array<Vector3> m_positions;
    Array<Vector3> m_normals;
    Array<Vector2> m_uvs;

    mutable G_VAO *m_vao = nullptr;
    G_VBO *m_vertexPositionsVBO = nullptr;
    G_VBO *m_vertexNormalsVBO = nullptr;
    G_VBO *m_vertexUvsVBO = nullptr;

    AABox m_bBox;
    Sphere m_bSphere;

    bool m_trisModel = false;
};

#endif // MESH_H
