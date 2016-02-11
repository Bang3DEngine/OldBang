#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

class Material : public GLBindable
{
friend class MeshRenderer;

private:
    const ShaderProgram *shaderProgram;
    const Texture *texture;

public:
    Material();
    virtual ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    void SetTexture(const Texture *texture);

    const ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // MATERIAL_H
