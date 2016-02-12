#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "FileLoader.h"
#include "Part.h"
#include "VAO.h"
#include "VBO.h"

#include <vector>

class Mesh : public Part
{
friend class MeshRenderer;

public:
    enum RenderMode
    {
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

private:
    VAO *vao;
    VBO *vertexPositionsVBO;
    VBO *vertexNormalsVBO;
    VBO *vertexUvsVBO;
    int vertexCount;

protected:
    Mesh::RenderMode renderMode;

public:
    Mesh();
    virtual ~Mesh();

    void LoadFromFile(const std::string &filepath);
    void LoadPositions(const std::vector<glm::vec3>& positions);
    void LoadNormals(const std::vector<glm::vec3>& normals);
    void LoadUvs(const std::vector<glm::vec2>& uvs);

    void SetRenderMode(RenderMode renderMode);

    VAO *GetVAO() const;
    RenderMode GetRenderMode() const;
    int GetVertexCount() const;
};

#endif // MESH_H
