#include "MeshScreenPlane.h"

MeshScreenPlane::MeshScreenPlane()
{
    LoadPositions(screenPlanePositions);
    LoadNormals(screenPlaneNormals);
    LoadUvs(screenPlaneUvs);
}

const std::vector<Vector3> MeshScreenPlane::screenPlanePositions =
{
    Vector3(-1.0f, -1.0f,  0.0f),
    Vector3( 1.0f, -1.0f,  0.0f),
    Vector3( 1.0f,  1.0f,  0.0f),

    Vector3(-1.0f, -1.0f,  0.0f),
    Vector3( 1.0f,  1.0f,  0.0f),
    Vector3(-1.0f,  1.0f,  0.0f)
};

const std::vector<Vector3> MeshScreenPlane::screenPlaneNormals =
{
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),

    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f)
};

const std::vector<glm::vec2> MeshScreenPlane::screenPlaneUvs =
{
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),

    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f)
};
