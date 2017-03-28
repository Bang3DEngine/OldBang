#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Color.h"

class Texture2D;
class ShaderProgram;
class Material : public Asset
{
    ASSET_ICON(Material, "Icons/MaterialAssetIcon.png")

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *mtexture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    ShaderProgram*   GetShaderProgram() const;
    const Texture2D* GetTexture() const;
    bool ReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    virtual void Bind() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    static Material* GetMissingMaterial();
private:
    Color m_diffuseColor = Color::White;
    float m_shininess = 60.0f;
    bool m_receivesLighting = true;

    ShaderProgram *m_shaderProgram = nullptr;
    const Texture2D *m_texture = nullptr; // Currently supporting just one texture

    friend class Renderer;
};

#endif // MATERIAL_H
