#ifndef MESHPLANE_H
#define MESHPLANE_H

#include "Bang.h"

#include "Mesh.h"

class MeshPlane : public Mesh
{
private:
    static const std::vector<glm::vec3> planePositions;
    static const std::vector<glm::vec3> planeNormals;
    static const std::vector<glm::vec2> planeUvs;

public:
    MeshPlane();
};

#endif // MESHPLANE_H
