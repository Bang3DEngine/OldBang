#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang.h"

#include <vector>

#include "Asset.h"
#include "Vector4.h"
#include "Texture.h"
#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

class Material : public IGLBindable
                ,public Asset
{
friend class Renderer;

public:
    const static std::string GetFileExtensionStatic() { return "bmat"; }
    const virtual std::string GetFileExtension()
    {
        return Material::GetFileExtensionStatic();
    }

private:
    Vector4 m_diffuseColor = Vector4(1.0f);

    ShaderProgram *m_shaderProgram = nullptr;
    const Texture2D *m_texture = nullptr; // Currently supporting just one texture

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *m_texture);

    void SetDiffuseColor(const Vector4 &m_diffuseColor);

    ShaderProgram* GetShaderProgram() const;
    const Texture* GetTexture() const;

    Vector4 GetDiffuseColor() const;

    void Bind() const override;
    void UnBind() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // MATERIAL_H
