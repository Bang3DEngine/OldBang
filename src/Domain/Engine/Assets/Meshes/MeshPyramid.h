#ifndef MESHPYRAMID_H
#define MESHPYRAMID_H

#include <vector>
#include "Mesh.h"

class MeshPyramid : public Mesh
{
public:
    static const std::vector<glm::vec3> pyramidPositions;
    static const std::vector<glm::vec3> pyramidNormals;
    static const std::vector<glm::vec2> pyramidUvs;

public:
    MeshPyramid();
};

#endif // MESHPYRAMID_H
