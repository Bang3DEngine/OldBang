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

    const ShaderProgram *p_shaderProgram;
    std::vector<Texture*> m_textures;
    mutable std::map<std::string, Texture*> m_namesToTextures;

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    void SetTexture(Texture *texture, unsigned int index = 0);
    void SetTexture(Texture *texture, const std::string &nameInShader);

    void SetDiffuseColor(const glm::vec4 &m_diffuseColor);

    const ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture(unsigned int index = 0) const;
    const Texture* GetTexture(const std::string &nameInShader) const;

    glm::vec4 GetDiffuseColor() const;

    void Bind() const override;
    void UnBind() const override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // MATERIAL_H
