#ifndef MESHPYRAMID_H
#define MESHPYRAMID_H

#include "Bang.h"

#include <vector>
#include "Mesh.h"

class MeshPyramid : public Mesh
{
public:
    static const std::vector<Vector3> pyramidPositions;
    static const std::vector<Vector3> pyramidNormals;
    static const std::vector<glm::vec2> pyramidUvs;

public:
    MeshPyramid();
};

#endif // MESHPYRAMID_H
