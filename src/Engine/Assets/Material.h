#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"

class Texture2D;
class G_ShaderProgram;
class Material : public Asset
{
	OBJECT(Material)
    ASSET_ICON(Material, "Icons/MaterialAssetIcon.png")

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(G_ShaderProgram *program);
    void SetTexture(const Texture2D *mtexture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2&   GetUvMultiply() const;
    G_ShaderProgram*   GetShaderProgram() const;
    const Texture2D* GetTexture() const;
    bool ReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    virtual void Bind() const;
    virtual void UnBind() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    static Material* GetMissingMaterial();

private:
    Color m_diffuseColor    = Color::White;
    float m_shininess       = 60.0f;
    bool m_receivesLighting = true;
    Vector2 m_uvMultiply    = Vector2::One;

    G_ShaderProgram *m_shaderProgram = nullptr;
    const Texture2D *m_texture     = nullptr;

    friend class Renderer;
};

#endif // MATERIAL_H
