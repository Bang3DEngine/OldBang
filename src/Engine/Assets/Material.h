#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Asset.h"

class Texture2D;
class G_Material;
class ShaderProgram;
class Material : public Asset
{
	OBJECT(Material)

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    virtual void CloneInto(ICloneable *clone) const override;

    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *texture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2&     GetUvMultiply() const;
    ShaderProgram*   GetShaderProgram() const;
    const Texture2D* GetTexture() const;
    bool ReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    void Bind() const;
    void UnBind() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    static Material* GetMissingMaterial();

private:
    const Texture2D *m_texture = nullptr;
    ShaderProgram *m_shaderProgram = nullptr;

    G_Material *m_gMaterial = nullptr; // Material delegate

    friend class Renderer;
};

#endif // MATERIAL_H
