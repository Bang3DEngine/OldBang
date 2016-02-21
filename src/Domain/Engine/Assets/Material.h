#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "Asset.h"
#include "Texture.h"
#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

class Material : public IGLBindable, public Asset
{
friend class MeshRenderer;

private:
    glm::vec4 diffuseColor;

    const ShaderProgram *shaderProgram;
    std::vector<const Texture*> textures;

public:
    Material();
    virtual ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    void SetTexture(const Texture *texture, unsigned int index = 0);

    void SetDiffuseColor(const glm::vec4 &diffuseColor);

    const ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture(unsigned int index = 0) const;

    glm::vec4 GetDiffuseColor() const;

    void Bind() const override;
    void UnBind() const override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // MATERIAL_H
