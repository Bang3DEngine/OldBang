#ifndef MATERIAL_H
#define MATERIAL_H

#include "Array.h"
#include "Asset.h"
#include "Color.h"

class Texture2D;
class ShaderProgram;
class Material : public Asset
{
public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

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

private:
    Color m_diffuseColor = Color::White;
    float m_shininess = 60.0f;

    ShaderProgram *m_shaderProgram = nullptr;
    const Texture2D *m_texture = nullptr; // Currently supporting just one texture

    friend class Renderer;
};

#endif // MATERIAL_H
