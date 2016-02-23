#ifndef MESHSCREENPLANE_H
#define MESHSCREENPLANE_H

#include "Bang.h"

#include "Mesh.h"

class MeshScreenPlane : public Mesh
{
private:
    static const std::vector<glm::vec3> screenPlanePositions;
    static const std::vector<glm::vec3> screenPlaneNormals;
    static const std::vector<glm::vec2> screenPlaneUvs;

public:
    MeshScreenPlane();
};

#endif // MESHSCREENPLANE_H
