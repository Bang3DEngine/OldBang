#include "Mesh.h"

Mesh::Mesh() : vertexPositionsVBO(nullptr), vertexNormalsVBO(nullptr)
{
    vao = new VAO();
}

Mesh::~Mesh()
{
    if(vertexPositionsVBO != nullptr) delete vertexPositionsVBO;
    if(vertexNormalsVBO != nullptr)   delete vertexNormalsVBO;
}

void Mesh::LoadVertices(const std::vector<glm::vec3>& vertices)
{
    if(vertexPositionsVBO != nullptr) delete vertexPositionsVBO;

    vertexPositionsVBO = new VBO();
    vertexPositionsVBO->Fill((void*)(&vertices[0]), vertices.size() * sizeof(float) * 3);
    vertexCount = vertices.size();
    vao->BindVBO(vertexPositionsVBO, VAO::VBOMeaning::Position);
}

void Mesh::LoadNormals(const std::vector<glm::vec3> &normals)
{
    if(vertexNormalsVBO != nullptr) delete vertexNormalsVBO;

    vertexNormalsVBO = new VBO();
    vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
    vao->BindVBO(vertexNormalsVBO, VAO::VBOMeaning::Normal);
}

VAO *Mesh::GetVAO() const
{
    return vao;
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}
