#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "Mesh.h"
#include "ShaderProgram.h"

class MeshRenderer
{
public:
    enum DrawingMode
    {
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

private:
    VAO *vao;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void Render(const ShaderProgram *shaderProgram, const Mesh *mesh,
                MeshRenderer::DrawingMode drawingMode) const;
};

#endif // MESHRENDERER_H
