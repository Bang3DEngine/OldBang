#ifndef MESHPYRAMID_H
#define MESHPYRAMID_H

#include <vector>
#include "Mesh.h"

class MeshPyramid : public Mesh
{
private:
    static const std::vector<glm::vec3> positions;
    static const std::vector<glm::vec3> normals;
    static const std::vector<glm::vec2> uvs;

public:
    MeshPyramid();
};

#endif // MESHPYRAMID_H
