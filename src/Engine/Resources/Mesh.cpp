#include "Bang/Mesh.h"

#include <GL/glew.h>

#include "Bang/IBO.h"
#include "Bang/VAO.h"
#include "Bang/VBO.h"
#include "Bang/Debug.h"
#include "Bang/ModelIO.h"
#include "Bang/Resources.h"
#include "Bang/XMLNodeReader.h"
#include "Bang/MeshSimplifier.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Mesh::Mesh() : m_bBox(Vector3::Zero)
{
    m_vao = new VAO();
}

Mesh::~Mesh()
{
    if (m_vao)                    { delete m_vao;                    }
    if (m_vertexIndicesIBO)       { delete m_vertexIndicesIBO;       }
    if (m_vertexPositionsPoolVBO) { delete m_vertexPositionsPoolVBO; }
    if (m_vertexNormalsPoolVBO)   { delete m_vertexNormalsPoolVBO;   }
    if (m_vertexUvsPoolVBO)       { delete m_vertexUvsPoolVBO;       }
}

void Mesh::LoadVertexIndices(const Array<Mesh::VertexId> &faceIndices)
{
    if (m_vertexIndicesIBO) { delete m_vertexIndicesIBO; }

    m_vertexIndices = faceIndices;
    if (m_vertexIndices.IsEmpty()) { m_vertexIndices.PushBack(0); }

    m_vertexIndicesIBO = new IBO();
    m_vertexIndicesIBO->Fill((void*)(&m_vertexIndices[0]),
                             m_vertexIndices.Size() * sizeof(Mesh::VertexId));

    m_vao->SetIBO(m_vertexIndicesIBO); // Bind to VAO
    m_areLodsValid = false;
}

void Mesh::LoadPositionsPool(const Array<Vector3>& positions)
{
    if (m_vertexPositionsPoolVBO) { delete m_vertexPositionsPoolVBO; }

    m_positionsPool = positions;
    if (m_positionsPool.IsEmpty())
    {
        m_positionsPool.PushBack( Vector3::Zero );
    }

    m_vertexPositionsPoolVBO = new VBO();
    m_vertexPositionsPoolVBO->Fill((void*)(&m_positionsPool[0]),
                               m_positionsPool.Size() * sizeof(float) * 3);
    BindPositionsVBOToLocation(Mesh::DefaultPositionsVBOLocation);

    m_bBox.CreateFromPositions(m_positionsPool);
    m_bSphere.FromBox(m_bBox);
    m_areLodsValid = false;
}

void Mesh::LoadNormalsPool(const Array<Vector3> &normals)
{
    if (m_vertexNormalsPoolVBO) { delete m_vertexNormalsPoolVBO; }

    m_normalsPool = normals;
    if (m_normalsPool.IsEmpty())
    {
        m_normalsPool.PushBack( Vector3::Zero );
    }

    m_vertexNormalsPoolVBO = new VBO();
    m_vertexNormalsPoolVBO->Fill((void*)(&m_normalsPool[0]),
                                     m_normalsPool.Size() * sizeof(float) * 3);
    BindNormalsVBOToLocation(Mesh::DefaultNormalsVBOLocation);
    m_areLodsValid = false;
}

void Mesh::LoadUvsPool(const Array<Vector2> &uvs)
{
    if (m_vertexUvsPoolVBO) { delete m_vertexUvsPoolVBO; }

    m_uvsPool = uvs;
    if (m_uvsPool.IsEmpty())
    {
        m_uvsPool.PushBack( Vector2::Zero );
    }

    m_vertexUvsPoolVBO = new VBO();
    m_vertexUvsPoolVBO->Fill((void*)(&m_uvsPool[0]), m_uvsPool.Size() * sizeof(float) * 2);
    BindUvsVBOToLocation(Mesh::DefaultUvsVBOLocation);
    m_areLodsValid = false;
}

void Mesh::LoadAll(const Array<Vector3> &positionsPool,
                   const Array<Vector3> &normalsPool,
                   const Array<Vector2> &uvsPool)
{
    LoadPositionsPool(positionsPool);
    LoadNormalsPool(normalsPool);
    LoadUvsPool(uvsPool);
}

void Mesh::LoadAll(const Array<Mesh::VertexId> &vertexIndices,
                   const Array<Vector3> &positionsPool,
                   const Array<Vector3> &normalsPool,
                   const Array<Vector2> &uvsPool)
{
    LoadVertexIndices(vertexIndices);
    LoadAll(positionsPool, normalsPool, uvsPool);
}

void Mesh::BindPositionsVBOToLocation(int positionsVBOLocation)
{
    m_vao->AddVBO(m_vertexPositionsPoolVBO, positionsVBOLocation, 3);
}

void Mesh::BindNormalsVBOToLocation(int normalsVBOLocation)
{
    m_vao->AddVBO(m_vertexNormalsPoolVBO, normalsVBOLocation, 3);
}

void Mesh::BindUvsVBOToLocation(int uvsVBOLocation)
{
    m_vao->AddVBO(m_vertexUvsPoolVBO, uvsVBOLocation, 2);
}

void Mesh::CalculateLODs()
{
    if (!m_areLodsValid)
    {
        m_lodMeshes = MeshSimplifier::GetAllMeshLODs( this );
        m_areLodsValid = true;
    }
}

RH<Mesh> Mesh::GetLOD(uint lod) const
{
    if (GetLODs().IsEmpty())
    {
        return RH<Mesh>(const_cast<Mesh*>(this));
    }

    const uint clampedLODLevel = Math::Min(lod, GetLODs().Size()-1);
    return GetLODs()[clampedLODLevel];
}

const Array<RH<Mesh> > Mesh::GetLODs() const
{
    return m_lodMeshes;
}

uint Mesh::GetNumTriangles() const
{
    return SCAST<uint>(GetVertexIndices().Size() / 3);
}

std::array<Mesh::VertexId, 3> Mesh::GetTriangleVertexIndices(int triIndex) const
{
    ASSERT(triIndex >= 0 && triIndex < GetNumTriangles());
    const VertexId triVertex0Index = GetVertexIndices()[triIndex * 3 + 0];
    const VertexId triVertex1Index = GetVertexIndices()[triIndex * 3 + 1];
    const VertexId triVertex2Index = GetVertexIndices()[triIndex * 3 + 2];
    return {{triVertex0Index, triVertex1Index, triVertex2Index}};
}

int Mesh::GetVertexCount() const
{
    return GetVertexIndicesIBO() ? GetVertexIndices().Size() :
                                   GetPositionsPool().Size();
}


VAO *Mesh::GetVAO() const { return m_vao; }
IBO *Mesh::GetVertexIndicesIBO() const { return m_vertexIndicesIBO; }
VBO *Mesh::GetVertexPositionsPoolVBO() const { return m_vertexPositionsPoolVBO; }
VBO *Mesh::GetVertexNormalsPoolVBO() const { return m_vertexNormalsPoolVBO; }
VBO *Mesh::GetVertexUvsPoolVBO() const { return m_vertexUvsPoolVBO; }
const AABox &Mesh::GetAABBox() const { return m_bBox; }
const Sphere &Mesh::GetBoundingSphere() const { return m_bSphere; }

const Array<Mesh::VertexId> &Mesh::GetVertexIndices() const { return m_vertexIndices; }
const Array<Vector3> &Mesh::GetPositionsPool() const { return m_positionsPool; }
const Array<Vector3> &Mesh::GetNormalsPool() const { return m_normalsPool; }
const Array<Vector2> &Mesh::GetUvsPool() const { return m_uvsPool; }

void Mesh::CloneInto(ICloneable *clone) const
{
    Mesh *mClone = Cast<Mesh*>(clone);

    mClone->m_bBox = m_bBox;
    mClone->m_bSphere = m_bSphere;

    if (mClone->m_vao) { delete mClone->m_vao; }
    mClone->m_vao = new VAO();
    mClone->LoadAll(m_vertexIndices, m_positionsPool, m_normalsPool, m_uvsPool);
}

void Mesh::Import(const Path &meshFilepath)
{
    Array<VertexId> vertexIndices;
    Array<Vector3> positionsPool;
    Array<Vector3> normalsPool;
    Array<Vector2> uvsPool;
    if ( ModelIO::ReadFirstFoundMeshRaw(meshFilepath,
                                        &vertexIndices,
                                        &positionsPool,
                                        &normalsPool,
                                        &uvsPool) )
    {
        LoadAll(vertexIndices, positionsPool, normalsPool, uvsPool);
    }
    else
    {
        Debug_Error("There was an error when reading mesh file '" <<
                     meshFilepath << "'.");
    }
}


void Mesh::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void Mesh::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}
