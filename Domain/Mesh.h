#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <GL/glew.h>
#include "VBO.h"

#include <vector>

class Mesh
{

private:
    VBO *verticesPosVBO;
    int vertexCount;

public:
    Mesh();
    virtual ~Mesh();

    void Load(const std::vector<glm::vec3>& vertices);

    int GetVertexCount() const;
};

#endif // MESH_H
