#ifndef MESH_H
#define MESH_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Sphere.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

FORWARD class VAO;
FORWARD class VBO;

class Mesh : public Asset
{
    ASSET(Mesh)

public:
    static constexpr uint DefaultPositionsVBOLocation = 0;
    static constexpr uint DefaultNormalsVBOLocation   = 1;
    static constexpr uint DefaultUvsVBOLocation       = 2;

    void LoadPositions(const Array<Vector3>& positions);
    void LoadNormals(const Array<Vector3>& normals);
    void LoadUvs(const Array<Vector2>& uvs);
    void LoadAll(const Array<Vector3>& positions,
                 const Array<Vector3>& normals,
                 const Array<Vector2>& uvs);

    void BindPositionsVBOToLocation(int positionsVBOLocation);
    void BindNormalsVBOToLocation(int normalsVBOLocation);
    void BindUvsVBOToLocation(int uvsVBOLocation);

    VAO *GetVAO() const;
    int GetVertexCount() const;
    const AABox& GetAABBox() const;
    const Sphere& GetBoundingSphere() const;
    const Array<Vector3>& GetPositions();
    const Array<Vector3>& GetNormals();
    const Array<Vector2>& GetUvs();
    const Path &GetModelFilepath() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Resource
    void Import(const Path &meshFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Array<Vector3> m_positions;
    Array<Vector3> m_normals;
    Array<Vector2> m_uvs;

    mutable VAO *m_vao = nullptr;
    VBO *m_vertexPositionsVBO = nullptr;
    VBO *m_vertexNormalsVBO = nullptr;
    VBO *m_vertexUvsVBO = nullptr;

    AABox m_bBox;
    Sphere m_bSphere;

    Mesh();
    virtual ~Mesh();
};

NAMESPACE_BANG_END

#endif // MESH_H
