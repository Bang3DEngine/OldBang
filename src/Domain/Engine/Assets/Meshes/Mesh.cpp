#include "Mesh.h"
#include "MeshPyramid.h"

Mesh::Mesh() : vertexPositionsVBO(nullptr), vertexNormalsVBO(nullptr), vertexUvsVBO(nullptr),
               renderMode(RenderMode::Triangles)
{
    vao = new VAO();
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

    bool trianglesMode;

    if( FileReader::ReadOBJ(filepath,
                            &positions, &normals, &uvs,
                            &trianglesMode))
    {
        LoadPositions(positions);
        LoadNormals(normals);
        LoadUvs(uvs);
        renderMode = trianglesMode ? RenderMode::Triangles : RenderMode::Quads;
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

void Mesh::BindAllVBOs(const ShaderProgram &sp)
{
    BindPositionsToShaderProgram(ShaderContract::Vertex_In_Position_Raw, sp);
    BindNormalsToShaderProgram(ShaderContract::Vertex_In_Normal_Raw, sp);
    BindUvsToShaderProgram(ShaderContract::Vertex_In_Uv_Raw, sp);
}

void Mesh::SetRenderMode(Mesh::RenderMode renderMode)
{
    this->renderMode = renderMode;
}

VAO *Mesh::GetVAO() const
{
    return vao;
}

Mesh::RenderMode Mesh::GetRenderMode() const
{
    return renderMode;
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}

void Mesh::Write(std::ofstream &f) const
{

}

void Mesh::Read(std::ifstream &f)
{
    filepath = FileReader::ReadString(f);
    LoadFromFile(filepath);
}
