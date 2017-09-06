#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Asset.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"

FORWARD class Texture2D;
FORWARD class ShaderProgram;

class Material : public Asset
{
    ASSET(Material)

public:
    Material();
    virtual ~Material();

    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(const Texture2D *mtexture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2& GetUvMultiply() const;
    ShaderProgram* GetShaderProgram() const;
    const Texture2D* GetTexture() const;
    bool IsReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    virtual void Bind() const;
    virtual void UnBind() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Resource
    void Import(const Path &materialFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    const Texture2D *m_texture     = nullptr;
    ShaderProgram *m_shaderProgram = nullptr;

    Color m_diffuseColor    = Color::White;
    float m_shininess       = 60.0f;
    bool m_receivesLighting = true;
    Vector2 m_uvMultiply    = Vector2::One;
};

#endif // MATERIAL_H
