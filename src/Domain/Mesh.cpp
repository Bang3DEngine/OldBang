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
    vertexPositionsVBO = new VBO();
    vertexPositionsVBO->Fill((void*)(&vertices[0]), vertices.size() * sizeof(float) * 3);
    vertexCount = vertices.size();
}

void Mesh::LoadNormals(const std::vector<glm::vec3> &normals)
{
    vertexNormalsVBO = new VBO();
    vertexNormalsVBO->Fill((void*)(&normals[0]), normals.size() * sizeof(float) * 3);
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}
