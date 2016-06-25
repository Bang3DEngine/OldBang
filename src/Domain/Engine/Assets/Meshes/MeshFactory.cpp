#include "MeshFactory.h"

void MeshFactory::GetPlaneTris(float s,
                           std::vector<Vector3> &positions,
                           std::vector<Vector3> &normals,
                           std::vector<glm::vec2> &uvs)
{
    positions =
    {
        s * Vector3(-0.5f, -0.5f,  0.0f),
        s * Vector3( 0.5f, -0.5f,  0.0f),
        s * Vector3( 0.5f,  0.5f,  0.0f),

        s * Vector3(-0.5f, -0.5f,  0.0f),
        s * Vector3( 0.5f,  0.5f,  0.0f),
        s * Vector3(-0.5f,  0.5f,  0.0f)
    };

    normals =
    {
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),

        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f)
    };

    uvs =
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),

        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };
}

void MeshFactory::GetCubeTris(float s,
                              std::vector<Vector3> &positions,
                              std::vector<Vector3> &normals,
                              std::vector<glm::vec2> &uvs)
{
    positions =
    {
        // FRONT
        s * Vector3(-0.5f, -0.5f, 0.5f),
        s * Vector3( 0.5f, -0.5f, 0.5f),
        s * Vector3( 0.5f,  0.5f, 0.5f),
        s * Vector3(-0.5f, -0.5f, 0.5f),
        s * Vector3( 0.5f,  0.5f, 0.5f),
        s * Vector3(-0.5f,  0.5f, 0.5f),

        // BACK
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3( 0.5f,  0.5f, -0.5f),
        s * Vector3( 0.5f, -0.5f, -0.5f),
        s * Vector3( 0.5f,  0.5f, -0.5f),
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3(-0.5f,  0.5f, -0.5f),

        // RIGHT
        s * Vector3(0.5f, -0.5f, -0.5f),
        s * Vector3(0.5f,  0.5f,  0.5f),
        s * Vector3(0.5f, -0.5f,  0.5f),
        s * Vector3(0.5f, -0.5f, -0.5f),
        s * Vector3(0.5f,  0.5f, -0.5f),
        s * Vector3(0.5f,  0.5f,  0.5f),

        // LEFT
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3(-0.5f, -0.5f,  0.5f),
        s * Vector3(-0.5f,  0.5f,  0.5f),
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3(-0.5f,  0.5f,  0.5f),
        s * Vector3(-0.5f,  0.5f, -0.5f),

        // TOP
        s * Vector3(-0.5f, 0.5f, -0.5f),
        s * Vector3(-0.5f, 0.5f,  0.5f),
        s * Vector3( 0.5f, 0.5f,  0.5f),
        s * Vector3(-0.5f, 0.5f, -0.5f),
        s * Vector3( 0.5f, 0.5f,  0.5f),
        s * Vector3( 0.5f, 0.5f, -0.5f),

        // BOT
        s * Vector3(-0.5f, -0.5f,  0.5f),
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3( 0.5f, -0.5f,  0.5f),
        s * Vector3( 0.5f, -0.5f,  0.5f),
        s * Vector3(-0.5f, -0.5f, -0.5f),
        s * Vector3( 0.5f, -0.5f, -0.5f)
    };

    normals =
    {
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),

        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),

        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),


        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),


        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),

        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f)
    };

    uvs =
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),

        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),

        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),

        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),

        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),

        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f)
    };
}
