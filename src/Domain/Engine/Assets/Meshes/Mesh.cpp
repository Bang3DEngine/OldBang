#include "Mesh.h"

Mesh::Mesh()
{
    m_vao = new VAO();
}

Mesh::Mesh(const Mesh &m)
{
    //TODO, do copy of VAO and VBO's
    m_vao = m.GetVAO();
    m_bBox = m.m_bBox;
    m_bSphere = m.m_bSphere;
    m_vertexPositionsVBO = m.m_vertexPositionsVBO;
    m_vertexNormalsVBO = m.m_vertexNormalsVBO;
    m_vertexUvsVBO = m.m_vertexUvsVBO;
    m_vertexCount = m.m_vertexCount;
}

Mesh::~Mesh()
{
    if (m_vertexPositionsVBO) delete m_vertexPositionsVBO;
    if (m_vertexNormalsVBO)   delete m_vertexNormalsVBO;
    if (m_vertexNormalsVBO)   delete m_vertexUvsVBO;
}

void Mesh::LoadFromFile(const std::string &filepath)
{
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<glm::vec2> uvs;

    if ( FileReader::ReadOBJ(filepath,
                            &positions, &normals, &uvs,
                            &m_trisModel))
    {
        LoadPositions(positions);
        LoadNormals(normals);
        LoadUvs(uvs);
    }
    else
    {
        Logger_Error("There was an error when reading mesh file '" << filepath << "'.");
    }
}

void Mesh::LoadPositions(const std::vector<Vector3>& positions)
{
    if (m_vertexPositionsVBO) delete m_vertexPositionsVBO;

    m_vertexPositionsVBO = new VBO();
    m_vertexPositionsVBO->Fill((void*)(&positions[0]), positions.size() * sizeof(float) * 3);
    m_vertexCount = positions.size();

    m_bBox.FillFromPositions(positions);
    m_bSphere.FillFromBox(m_bBox);
}

void Mesh::LoadNormals(const std::vector<Vector3> &normals)
{
    if (m_vertexNormalsVBO) delete m_vertexNormalsVBO;

    m_vertexNormalsVBO = new VBO();
    m_vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
}

void Mesh::LoadUvs(const std::vector<glm::vec2> &uvs)
{
    if (m_vertexUvsVBO) delete m_vertexUvsVBO;

    m_vertexUvsVBO = new VBO();
    m_vertexUvsVBO->Fill((void*)(&uvs[0]), uvs.size() * sizeof(float) * 2);
}

void Mesh::LoadAll(const std::vector<Vector3> &positions,
                   const std::vector<Vector3> &normals,
                   const std::vector<glm::vec2> &uvs)
{
    LoadPositions(positions);
    LoadNormals(normals);
    LoadUvs(uvs);
}

void Mesh::BindPositionsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexPositionsVBO )
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexPositionsVBO, location, 3);
    }
}

void Mesh::BindNormalsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexNormalsVBO )
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexNormalsVBO, location, 3);
    }
}

void Mesh::BindUvsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexUvsVBO )
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexUvsVBO, location, 2);
    }
}

void Mesh::BindAllVBOsToShaderProgram(const ShaderProgram &sp)
{
    BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw, sp);
    BindNormalsToShaderProgram(ShaderContract::Attr_Vertex_In_Normal_Raw, sp);
    BindUvsToShaderProgram(ShaderContract::Attr_Vertex_In_Uv_Raw, sp);
}

VAO *Mesh::GetVAO() const
{
    return m_vao;
}

int Mesh::GetVertexCount() const
{
    return m_vertexCount;
}

bool Mesh::IsATrianglesModel() const
{
    return m_trisModel;
}

const Box &Mesh::GetBoundingBox() const
{
    return m_bBox;
}

const Sphere &Mesh::GetBoundingSphere() const
{
    return m_bSphere;
}

void Mesh::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);
    m_filepath = xmlInfo->GetString("Mesh");
    LoadFromFile(m_filepath);
}

void Mesh::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Mesh");
}
