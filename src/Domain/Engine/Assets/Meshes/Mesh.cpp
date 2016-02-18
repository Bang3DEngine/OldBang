#include "Mesh.h"

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

    FileReader::ReadOBJ(filepath,
                        &positions, &normals, &uvs,
                        &trianglesMode);
    renderMode = trianglesMode ? RenderMode::Triangles : RenderMode::Quads;
}

void Mesh::LoadPositions(const std::vector<glm::vec3>& positions)
{
    if(vertexPositionsVBO != nullptr) delete vertexPositionsVBO;

    vertexPositionsVBO = new VBO();
    vertexPositionsVBO->Fill((void*)(&positions[0]), positions.size() * sizeof(float) * 3);
    vertexCount = positions.size();
    vao->BindVBO(vertexPositionsVBO, VAO::VBOMeaning::Position);
}

void Mesh::LoadNormals(const std::vector<glm::vec3> &normals)
{
    if(vertexNormalsVBO != nullptr) delete vertexNormalsVBO;

    vertexNormalsVBO = new VBO();
    vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
    vao->BindVBO(vertexNormalsVBO, VAO::VBOMeaning::Normal);
}

void Mesh::LoadUvs(const std::vector<glm::vec2> &uvs)
{
    if(vertexUvsVBO != nullptr) delete vertexUvsVBO;

    vertexUvsVBO = new VBO();
    vertexUvsVBO->Fill((void*)(&uvs[0]), uvs.size() * sizeof(float) * 2);
    vao->BindVBO(vertexUvsVBO, VAO::VBOMeaning::UV);
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
    StageReader::RegisterNextPointer(f, this);
    std::string filepath = StageReader::ReadString(f);
    LoadFromFile(filepath);
    StageReader::GetLine(f); //Consume close tag
}
