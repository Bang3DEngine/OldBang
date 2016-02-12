#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "Texture.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

class Material : public GLBindable
{
friend class MeshRenderer;

private:
    const ShaderProgram *shaderProgram;
    std::vector<const Texture*> textures;

public:
    Material();
    virtual ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    void SetTexture(const Texture *texture, unsigned int index = 0);

    const ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture(unsigned int index = 0) const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // MATERIAL_H
