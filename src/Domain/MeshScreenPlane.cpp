#include "MeshScreenPlane.h"

MeshScreenPlane::MeshScreenPlane()
{
    LoadPositions(screenPlanePositions);
    LoadNormals(screenPlaneNormals);
    LoadUvs(screenPlaneUvs);
}

const std::vector<glm::vec3> MeshScreenPlane::screenPlanePositions =
{
    glm::vec3(-1.0f, -1.0f,  0.0f),
    glm::vec3( 1.0f, -1.0f,  0.0f),
    glm::vec3( 1.0f,  1.0f,  0.0f),

    glm::vec3(-1.0f, -1.0f,  0.0f),
    glm::vec3( 1.0f,  1.0f,  0.0f),
    glm::vec3(-1.0f,  1.0f,  0.0f)
};

const std::vector<glm::vec3> MeshScreenPlane::screenPlaneNormals =
{
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),

    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
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
