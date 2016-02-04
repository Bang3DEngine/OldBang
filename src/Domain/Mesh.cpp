#include "Mesh.h"

Mesh::Mesh() : verticesPosVBO(nullptr)
{
}

Mesh::~Mesh()
{
    if(verticesPosVBO != nullptr) delete verticesPosVBO;
}

void Mesh::Load(const std::vector<glm::vec3>& vertices)
{
    verticesPosVBO = new VBO();
    verticesPosVBO->Fill((void*)(&vertices[0]), vertices.size() * sizeof(float) * 3);
    vertexCount = vertices.size();
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}
