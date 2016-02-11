#include "MeshPlane.h"

MeshPlane::MeshPlane()
{
    LoadPositions(planePositions);
    LoadNormals(planeNormals);
    LoadUvs(planeUvs);
}

const std::vector<glm::vec3> MeshPlane::planePositions =
{
    glm::vec3(-0.5f, -0.5f,  0.0f),
    glm::vec3( 0.5f, -0.5f,  0.0f),
    glm::vec3( 0.5f,  0.5f,  0.0f),

    glm::vec3(-0.5f, -0.5f,  0.0f),
    glm::vec3( 0.5f,  0.5f,  0.0f),
    glm::vec3(-0.5f,  0.5f,  0.0f)
};

const std::vector<glm::vec3> MeshPlane::planeNormals =
{
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),

    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
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
