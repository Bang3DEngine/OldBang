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
private:

    Mesh *mesh;
    const Material *material;

protected:

    void _OnRender() override;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void Render(Mesh::RenderMode drawingMode = Mesh::RenderMode::Triangles) const;

    void SetMesh(Mesh *m);
    const Mesh* GetMesh();

    void SetMaterial(const Material *m);
    const Material* GetMaterial();

    void Write(std::ofstream &f) const;
    void Read(std::ifstream &f);
};

#endif // MESHRENDERER_H
