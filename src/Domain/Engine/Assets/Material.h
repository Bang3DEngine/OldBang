#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang.h"

#include "Array.h"

#include "Asset.h"
#include "Color.h"
#include "Vector4.h"
#include "Texture.h"
#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

class Material : public Asset
{
public:
    const static String GetFileExtensionStatic() { return "bmat"; }
    const virtual String GetFileExtension()
    {
        return Material::GetFileExtensionStatic();
    }

private:
    Color m_diffuseColor = Color::white;
    float m_shininess = 60.0f;

    ShaderProgram *m_shaderProgram = nullptr;
    const Texture2D *m_texture = nullptr; // Currently supporting just one texture

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *mtexture);

    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    ShaderProgram*   GetShaderProgram() const;
    const Texture2D* GetTexture() const;

    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    void Bind() const;
    void UnBind() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    friend class Renderer;
};

#endif // MATERIAL_H
