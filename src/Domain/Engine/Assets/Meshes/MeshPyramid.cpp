#include "MeshPyramid.h"


MeshPyramid::MeshPyramid()
{
    LoadPositions(pyramidPositions);
    LoadNormals(pyramidNormals);
    LoadUvs(pyramidUvs);
}

const std::vector<Vector3> MeshPyramid::pyramidPositions =
{
    //RIGHT
    Vector3(0.0f,  0.5f,  0.0f),
    Vector3(0.5f, -0.5f,  0.5f),
    Vector3(0.5f, -0.5f, -0.5f),

    //LEFT
    Vector3( 0.0f,  0.5f,  0.0f),
    Vector3(-0.5f, -0.5f, -0.5f),
    Vector3(-0.5f, -0.5f,  0.5f),

    //FRONT
    Vector3( 0.0f,  0.5f, 0.0f),
    Vector3(-0.5f, -0.5f, 0.5f),
    Vector3( 0.5f, -0.5f, 0.5f),

    //BACK
    Vector3( 0.0f,  0.5f,  0.0f),
    Vector3( 0.5f, -0.5f, -0.5f),
    Vector3(-0.5f, -0.5f, -0.5f),

    //BOTTOM FRONT
    Vector3(-0.5f, -0.5f, -0.5f),
    Vector3( 0.5f, -0.5f, -0.5f),
    Vector3( 0.5f, -0.5f,  0.5f),

    //BOTTOM BACK
    Vector3(-0.5f, -0.5f, -0.5f),
    Vector3( 0.5f, -0.5f,  0.5f),
    Vector3(-0.5f, -0.5f,  0.5f)
};

const std::vector<Vector3> MeshPyramid::pyramidNormals =
{
    //RIGHT
    Vector3(1.0f,  0.0f,  0.0f),
    Vector3(1.0f,  0.0f,  0.0f),
    Vector3(1.0f,  0.0f,  0.0f),

    //LEFT
    Vector3(-1.0f,  0.0f,  0.0f),
    Vector3(-1.0f,  0.0f,  0.0f),
    Vector3(-1.0f,  0.0f,  0.0f),

    //FRONT
    Vector3(0.0f,  0.0f, -1.0f),
    Vector3(0.0f,  0.0f, -1.0f),
    Vector3(0.0f,  0.0f, -1.0f),

    //BACK
    Vector3(0.0f,  0.0f,  1.0f),
    Vector3(0.0f,  0.0f,  1.0f),
    Vector3(0.0f,  0.0f,  1.0f),

    //BOTTOM FRONT
    Vector3(0.0f, -1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f),

    //BOTTOM BACK
    Vector3(0.0f, -1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f)
};

const std::vector<glm::vec2> MeshPyramid::pyramidUvs =
{
    //RIGHT
    glm::vec2(0.125f, 1.0f),
    glm::vec2(0.125f, 0.0f),
    glm::vec2(0.25f,  0.0f),

    //LEFT
    glm::vec2(0.625f, 1.0f),
    glm::vec2(0.5f,   0.0f),
    glm::vec2(0.75f,  0.0f),

    //FRONT
    glm::vec2(0.875f, 1.0f),
    glm::vec2(0.75f,  0.0f),
    glm::vec2(1.0f,   0.0f),

    //BACK
    glm::vec2(0.375f, 1.0f),
    glm::vec2(0.25f,  0.0f),
    glm::vec2(0.5f,   0.0f),

    //BOTTOM FRONT
    glm::vec2(0.5f,  0.0f),
    glm::vec2(0.75f, 0.0f),
    glm::vec2(1.0f,  0.0f),

    //BOTTOM BACK
    glm::vec2(0.5f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(0.25f, 0.0f)
};
