#include "MeshPlane.h"

MeshPlane::MeshPlane()
{
    LoadPositions(planePositions);
    LoadNormals(planeNormals);
    LoadUvs(planeUvs);
}

const std::vector<Vector3> MeshPlane::planePositions =
{
    Vector3(-0.5f, -0.5f,  0.0f),
    Vector3( 0.5f, -0.5f,  0.0f),
    Vector3( 0.5f,  0.5f,  0.0f),

    Vector3(-0.5f, -0.5f,  0.0f),
    Vector3( 0.5f,  0.5f,  0.0f),
    Vector3(-0.5f,  0.5f,  0.0f)
};

const std::vector<Vector3> MeshPlane::planeNormals =
{
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),

    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, 1.0f)
};

const std::vector<glm::vec2> MeshPlane::planeUvs =
{
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),

    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f)
};
