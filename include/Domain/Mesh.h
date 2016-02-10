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
    VBO *vertexUvsVBO;
    int vertexCount;

public:
    Mesh();
    virtual ~Mesh();

    void LoadPositions(const std::vector<glm::vec3>& positions);
    void LoadNormals(const std::vector<glm::vec3>& normals);
    void LoadUvs(const std::vector<glm::vec2>& uvs);

    VAO *GetVAO() const;
    int GetVertexCount() const;
};

#endif // MESH_H
