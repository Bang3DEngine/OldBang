#include "Bang/Mesh.h"

#include <GL/glew.h>

#include "Bang/G_VAO.h"
#include "Bang/G_VBO.h"
#include "Bang/XMLNode.h"
#include "Bang/FileReader.h"
#include "Bang/SerializableObject.h"

Mesh::Mesh()
{
    m_vao = new G_VAO();
}

Mesh::Mesh(const Mesh &m)
{
    m_vao = m.GetVAO();
    m_bBox = m.m_bBox;
    m_bSphere = m.m_bSphere;
    m_vertexPositionsVBO = m.m_vertexPositionsVBO;
    m_vertexNormalsVBO = m.m_vertexNormalsVBO;
    m_vertexUvsVBO = m.m_vertexUvsVBO;
    m_positions = m.m_positions;
    m_normals = m.m_normals;
    m_uvs = m.m_uvs;
}

Mesh::~Mesh()
{
    if (m_vao)                { delete m_vao;                }
    if (m_vertexPositionsVBO) { delete m_vertexPositionsVBO; }
    if (m_vertexNormalsVBO)   { delete m_vertexNormalsVBO;   }
    if (m_vertexUvsVBO)       { delete m_vertexUvsVBO;       }
}

String Mesh::GetFileExtensionStatic()
{
    return "bmesh";
}

String Mesh::GetFileExtension() const
{
    return Mesh::GetFileExtensionStatic();
}

void Mesh::LoadFromFile(const Path &filepath)
{
    m_uvs.Clear();
    m_normals.Clear();
    m_positions.Clear();
    if ( FileReader::ReadModel(filepath, &m_positions, &m_normals, &m_uvs))
    {
        LoadPositions(m_positions);
        LoadNormals(m_normals);
        LoadUvs(m_uvs);
    }
    else
    {
        Debug_Error("There was an error when reading mesh file '" <<
                    filepath << "'.");
    }
}

void Mesh::LoadPositions(const Array<Vector3>& positions)
{
    if (m_vertexPositionsVBO) { delete m_vertexPositionsVBO; }

    m_positions = positions;
    if (m_positions.IsEmpty())
    {
        m_positions.PushBack( Vector3::Zero );
    }

    m_vertexPositionsVBO = new G_VBO();
    m_vertexPositionsVBO->Fill((void*)(&m_positions[0]),
                               m_positions.Size() * sizeof(float) * 3);
    BindPositionsVBOToLocation(Mesh::DefaultPositionsVBOLocation);

    m_bBox.FillFromPositions(m_positions);
    m_bSphere.FromBox(m_bBox);
}

void Mesh::LoadNormals(const Array<Vector3> &normals)
{
    if (m_vertexNormalsVBO) delete m_vertexNormalsVBO;

    m_normals = normals;
    if (m_normals.IsEmpty())
    {
        m_normals.PushBack( Vector3::Zero );
    }
    m_vertexNormalsVBO = new G_VBO();
    m_vertexNormalsVBO->Fill((void*)(&m_normals[0]),
                                     m_normals.Size() * sizeof(float) * 3);
    BindNormalsVBOToLocation(Mesh::DefaultNormalsVBOLocation);
}

void Mesh::LoadUvs(const Array<Vector2> &uvs)
{
    if (m_vertexUvsVBO) delete m_vertexUvsVBO;

    m_uvs = uvs;
    if (m_uvs.IsEmpty())
    {
        m_uvs.PushBack( Vector2::Zero );
    }
    m_vertexUvsVBO = new G_VBO();
    m_vertexUvsVBO->Fill((void*)(&m_uvs[0]), m_uvs.Size() * sizeof(float) * 2);
    BindUvsVBOToLocation(Mesh::DefaultUvsVBOLocation);
}

void Mesh::LoadAll(const Array<Vector3> &positions,
                   const Array<Vector3> &normals,
                   const Array<Vector2> &uvs)
{
    LoadPositions(positions);
    LoadNormals(normals);
    LoadUvs(uvs);
}

void Mesh::BindPositionsVBOToLocation(int positionsVBOLocation)
{
    m_vao->BindVBO(m_vertexPositionsVBO, positionsVBOLocation, 3);
}

void Mesh::BindNormalsVBOToLocation(int normalsVBOLocation)
{
    m_vao->BindVBO(m_vertexNormalsVBO, normalsVBOLocation, 3);
}

void Mesh::BindUvsVBOToLocation(int uvsVBOLocation)
{
    m_vao->BindVBO(m_vertexUvsVBO, uvsVBOLocation, 2);
}

G_VAO *Mesh::GetVAO() const
{
    return m_vao;
}

int Mesh::GetVertexCount() const
{
    return m_positions.Size();
}

const AABox &Mesh::GetAABBox() const
{
    return m_bBox;
}

const Sphere &Mesh::GetBoundingSphere() const
{
    return m_bSphere;
}

const Array<Vector3> &Mesh::GetPositions()
{
    return m_positions;
}

const Array<Vector3> &Mesh::GetNormals()
{
    return m_normals;
}

const Array<Vector2> &Mesh::GetUvs()
{
    return m_uvs;
}

const Path &Mesh::GetModelFilepath() const
{
    return m_modelFilepath;
}

void Mesh::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);
    m_modelFilepath = xmlInfo.GetFilepath("ModelFilepath");
    LoadFromFile(m_modelFilepath);
}

void Mesh::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);
    xmlInfo->SetFilepath("ModelFilepath", m_modelFilepath);
}

