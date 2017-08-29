#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Asset.h"

FORWARD   class Texture2D;
FORWARD   class G_Material;
FORWARD   class ShaderProgram;

class Material : public Asset
{
    ASSET(Material)

public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    void Bind() const;
    void UnBind() const;

    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *texture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2& GetUvMultiply() const;
    ShaderProgram* GetShaderProgram() const;
    const Texture2D* GetTexture() const;
    bool IsReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Resource
    void Import(const Path &materialFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    const Texture2D *m_texture = nullptr;
    ShaderProgram *m_shaderProgram = nullptr;

    G_Material *m_gMaterial = nullptr; // Material delegate
};

#endif // MATERIAL_H
