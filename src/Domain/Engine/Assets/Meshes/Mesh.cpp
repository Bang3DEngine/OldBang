#include "Mesh.h"
#include "MeshPyramid.h"

Mesh::Mesh()
{
    vao = new VAO();
}

Mesh::Mesh(const Mesh &m)
{
    //TODO, do copy of VAO and VBO's
    vao = m.GetVAO();
    bbox = m.bbox;
    vertexPositionsVBO = m.vertexPositionsVBO;
    vertexNormalsVBO = m.vertexNormalsVBO;
    vertexUvsVBO = m.vertexUvsVBO;
    vertexCount = m.vertexCount;
}

Mesh::~Mesh()
{
    if(vertexPositionsVBO != nullptr) delete vertexPositionsVBO;
    if(vertexNormalsVBO != nullptr)   delete vertexNormalsVBO;
    if(vertexNormalsVBO != nullptr)   delete vertexUvsVBO;
}

void Mesh::LoadFromFile(const std::string &filepath)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    if( FileReader::ReadOBJ(filepath,
                            &positions, &normals, &uvs,
                            &trisModel))
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

void Mesh::LoadPositions(const std::vector<glm::vec3>& positions)
{
    if(vertexPositionsVBO != nullptr) delete vertexPositionsVBO;

    vertexPositionsVBO = new VBO();
    vertexPositionsVBO->Fill((void*)(&positions[0]), positions.size() * sizeof(float) * 3);
    vertexCount = positions.size();

    bbox.FillFromBoundingBox(positions);
}

void Mesh::LoadNormals(const std::vector<glm::vec3> &normals)
{
    if(vertexNormalsVBO != nullptr) delete vertexNormalsVBO;

    vertexNormalsVBO = new VBO();
    vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
}

void Mesh::LoadUvs(const std::vector<glm::vec2> &uvs)
{
    if(vertexUvsVBO != nullptr) delete vertexUvsVBO;

    vertexUvsVBO = new VBO();
    vertexUvsVBO->Fill((void*)(&uvs[0]), uvs.size() * sizeof(float) * 2);
}

void Mesh::BindPositionsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if(vertexPositionsVBO != nullptr)
    {
        GLint location = sp.GetLocation(nameInShader);
        vao->BindVBO(vertexPositionsVBO, location, 3);
    }
}

void Mesh::BindNormalsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if(vertexNormalsVBO != nullptr)
    {
        GLint location = sp.GetLocation(nameInShader);
        vao->BindVBO(vertexNormalsVBO, location, 3);
    }
}

void Mesh::BindUvsToShaderProgram(const std::string &nameInShader, const ShaderProgram &sp)
{
    if(vertexUvsVBO != nullptr)
    {
        GLint location = sp.GetLocation(nameInShader);
        vao->BindVBO(vertexUvsVBO, location, 2);
    }
}

void Mesh::BindAllVBOsToShaderProgram(const ShaderProgram &sp)
{
    BindPositionsToShaderProgram(ShaderContract::Vertex_In_Position_Raw, sp);
    BindNormalsToShaderProgram(ShaderContract::Vertex_In_Normal_Raw, sp);
    BindUvsToShaderProgram(ShaderContract::Vertex_In_Uv_Raw, sp);
}

VAO *Mesh::GetVAO() const
{
    return vao;
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}

bool Mesh::IsATrianglesModel() const
{
    return trisModel;
}

const Box &Mesh::GetBoundingBox() const
{
    return bbox;
}

void Mesh::Write(std::ostream &f) const
{

}

void Mesh::Read(std::istream &f)
{
    filepath = FileReader::ReadString(f);
    LoadFromFile(filepath);
}
