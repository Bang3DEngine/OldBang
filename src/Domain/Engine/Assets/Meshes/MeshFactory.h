#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <vector>

#include "Vector3.h"

class MeshFactory
{
private:
    MeshFactory() {}

public:

    static
    void GetPlaneTris(float size,
                      std::vector<Vector3> &positions,
                      std::vector<Vector3> &normals,
                      std::vector<glm::vec2> &uvs);
    static
    void GetCubeTris(float size,
                     std::vector<Vector3> &positions,
                     std::vector<Vector3> &normals,
                     std::vector<glm::vec2> &uvs);

};

#endif // MESHFACTORY_H
