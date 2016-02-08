#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <GL/glew.h>
#include "Part.h"
#include "VAO.h"
#include "VBO.h"

#include <vector>

class Mesh : public Part
{
friend class MeshRenderer;

private:
    VAO *vao;
    VBO *vertexPositionsVBO;
    VBO *vertexNormalsVBO;
    int vertexCount;

public:
    Mesh();
    virtual ~Mesh();

    void LoadVertices(const std::vector<glm::vec3>& vertices);
    void LoadNormals(const std::vector<glm::vec3>& normals);

    VAO *GetVAO() const;
    int GetVertexCount() const;
};

#endif // MESH_H
