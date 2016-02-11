#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

class Material : public GLBindable
{
friend class MeshRenderer;

private:
    const ShaderProgram *shaderProgram;
    const Texture2D *texture;

public:
    Material();
    virtual ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    void SetTexture(const Texture2D *texture);

    const ShaderProgram* GetShaderProgram() const;
    const Texture2D* GetTexture() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // MATERIAL_H
