#ifndef MESH_H
#define MESH_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Sphere.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

FORWARD class IBO;
FORWARD class VAO;
FORWARD class VBO;

class Mesh : public Asset,
             public ICloneable
{
    ASSET(Mesh)

public:
    using VertexId = uint;

    static constexpr uint DefaultPositionsVBOLocation = 0;
    static constexpr uint DefaultNormalsVBOLocation   = 1;
    static constexpr uint DefaultUvsVBOLocation       = 2;

    void LoadVertexIndices(const Array<VertexId>& vertexIndices);
    void LoadPositionsPool(const Array<Vector3>& positions);
    void LoadNormalsPool(const Array<Vector3>& normals);
    void LoadUvsPool(const Array<Vector2>& uvs);
    void LoadAll(const Array<Vector3>& positionsPool,
                 const Array<Vector3>& normalsPool,
                 const Array<Vector2>& uvsPool);
    void LoadAll(const Array<VertexId>& vertexIndices,
                 const Array<Vector3>& positionsPool,
                 const Array<Vector3>& normalsPool,
                 const Array<Vector2>& uvsPool);

    void BindPositionsVBOToLocation(int positionsVBOLocation);
    void BindNormalsVBOToLocation(int normalsVBOLocation);
    void BindUvsVBOToLocation(int uvsVBOLocation);

    void CalculateLODs();
    RH<Mesh> GetLOD(uint lod) const;
    const Array<RH<Mesh>> GetLODs() const;
    uint GetNumTriangles() const;
    std::array<VertexId, 3> GetTriangleVertexIndices(int triIndex) const;

    VAO *GetVAO() const;
    IBO *GetVertexIndicesIBO() const;
    VBO *GetVertexPositionsPoolVBO() const;
    VBO *GetVertexNormalsPoolVBO() const;
    VBO *GetVertexUvsPoolVBO() const;
    int GetVertexCount() const;
    const AABox& GetAABBox() const;
    const Sphere& GetBoundingSphere() const;
    const Array<VertexId>& GetVertexIndices() const;
    const Array<Vector3>& GetPositionsPool() const;
    const Array<Vector3>& GetNormalsPool() const;
    const Array<Vector2>& GetUvsPool() const;
    const Path &GetModelFilepath() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Resource
    void Import(const Path &meshFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    bool m_areLodsValid = false;
    Array< RH<Mesh> > m_lodMeshes;
    Array<VertexId> m_vertexIndices;
    Array<Vector3> m_positionsPool;
    Array<Vector3> m_normalsPool;
    Array<Vector2> m_uvsPool;

    mutable VAO *m_vao = nullptr;
    IBO *m_vertexIndicesIBO = nullptr;
    VBO *m_vertexPositionsPoolVBO = nullptr;
    VBO *m_vertexNormalsPoolVBO = nullptr;
    VBO *m_vertexUvsPoolVBO = nullptr;

    AABox m_bBox;
    Sphere m_bSphere;

    Mesh();
    virtual ~Mesh();
};

NAMESPACE_BANG_END

#endif // MESH_H
