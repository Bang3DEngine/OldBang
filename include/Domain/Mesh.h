#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <GL/glew.h>
#include "VBO.h"

#include <vector>

class Mesh
{
friend class MeshRenderer;

private:
    VBO *vertexPositionsVBO;
    VBO *vertexNormalsVBO;
    int vertexCount;

public:
    Mesh();
    virtual ~Mesh();

    void LoadVertices(const std::vector<glm::vec3>& vertices);
    void LoadNormals(const std::vector<glm::vec3>& normals);

    int GetVertexCount() const;
};

#endif // MESH_H
