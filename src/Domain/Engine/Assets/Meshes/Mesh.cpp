#include "Mesh.h"
#include "MeshPyramid.h"

Mesh::Mesh()
{
    p_vao = new VAO();
}

Mesh::Mesh(const Mesh &m)
{
    //TODO, do copy of VAO and VBO's
    p_vao = m.GetVAO();
    m_bBox = m.m_bBox;
    m_bSphere = m.m_bSphere;
    p_vertexPositionsVBO = m.p_vertexPositionsVBO;
    p_vertexNormalsVBO = m.p_vertexNormalsVBO;
    p_vertexUvsVBO = m.p_vertexUvsVBO;
    m_vertexCount = m.m_vertexCount;
}

Mesh::~Mesh()
{
    if (p_vertexPositionsVBO) delete p_vertexPositionsVBO;
    if (p_vertexNormalsVBO)   delete p_vertexNormalsVBO;
    if (p_vertexNormalsVBO)   delete p_vertexUvsVBO;
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
    if (p_vertexPositionsVBO) delete p_vertexPositionsVBO;

    p_vertexPositionsVBO = new VBO();
    p_vertexPositionsVBO->Fill((void*)(&positions[0]), positions.size() * sizeof(float) * 3);
    m_vertexCount = positions.size();

    m_bBox.FillFromPositions(positions);
    m_bSphere.FillFromBox(m_bBox);
}

void Mesh::LoadNormals(const std::vector<Vector3> &normals)
{
    if (p_vertexNormalsVBO) delete p_vertexNormalsVBO;

    p_vertexNormalsVBO = new VBO();
    p_vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
}

void Mesh::LoadUvs(const std::vector<glm::vec2> &uvs)
{
    if (p_vertexUvsVBO) delete p_vertexUvsVBO;

    p_vertexUvsVBO = new VBO();
    p_vertexUvsVBO->Fill((void*)(&uvs[0]), uvs.size() * sizeof(float) * 2);
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
    if (p_vertexPositionsVBO )
    {
        GLint location = sp.GetLocation(nameInShader);
        p_vao->BindVBO(p_vertexPositionsVBO, location, 3);
    }
}

void Mesh::BindNormalsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if (p_vertexNormalsVBO )
    {
        GLint location = sp.GetLocation(nameInShader);
        p_vao->BindVBO(p_vertexNormalsVBO, location, 3);
    }
}

void Mesh::BindUvsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if (p_vertexUvsVBO )
    {
        GLint location = sp.GetLocation(nameInShader);
        p_vao->BindVBO(p_vertexUvsVBO, location, 2);
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
    return p_vao;
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

void Mesh::Write(std::ostream &f) const
{

}

void Mesh::Read(std::istream &f)
{
    m_filepath = FileReader::ReadString(f);
    LoadFromFile(m_filepath);
}
