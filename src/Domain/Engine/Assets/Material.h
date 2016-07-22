#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang.h"

#include <vector>

#include "Asset.h"
#include "Texture.h"
#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

class Material : public IGLBindable, public Asset
{
friend class Renderer;

public:
    const static std::string GetFileExtensionStatic() { return "bmat"; }
    const virtual std::string GetFileExtension()
    {
        return Material::GetFileExtensionStatic();
    }

private:
    glm::vec4 m_diffuseColor;

    ShaderProgram *p_shaderProgram = nullptr;
    Texture2D *m_texture = nullptr; // Currently supporting just one texture

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(Texture2D *m_texture);

    void SetDiffuseColor(const glm::vec4 &m_diffuseColor);

    ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture() const;

    glm::vec4 GetDiffuseColor() const;

    void Bind() const override;
    void UnBind() const override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // MATERIAL_H
