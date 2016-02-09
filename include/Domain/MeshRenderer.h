#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "Mesh.h"
#include "Part.h"
#include "Stage.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include <glm/gtx/string_cast.hpp>

class MeshRenderer : public Part
{
public:
    enum RenderMode
    {
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

private:

    const Mesh *mesh;
    const Material *material;

protected:

    void _OnRender() override;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void Render(const Transform *t,
                MeshRenderer::RenderMode drawingMode = MeshRenderer::RenderMode::Triangles) const;

    void SetMesh(const Mesh *m);
    const Mesh* GetMesh();

    void SetMaterial(const Material *m);
    const Material* GetMaterial();
};

#endif // MESHRENDERER_H
