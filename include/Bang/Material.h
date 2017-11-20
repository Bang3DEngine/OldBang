#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Asset.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;
FORWARD class ShaderProgram;

class Material : public Asset
{
    ASSET(Material)

public:
    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(Texture2D *mtexture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2& GetUvMultiply() const;
    ShaderProgram* GetShaderProgram() const;
    Texture2D* GetTexture() const;
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
    Texture2D *m_texture     = nullptr;
    ShaderProgram *m_shaderProgram = nullptr;

    Color m_diffuseColor    = Color::White;
    float m_shininess       = 60.0f;
    bool m_receivesLighting = true;
    Vector2 m_uvMultiply    = Vector2::One;

    Material();
    virtual ~Material();
};

NAMESPACE_BANG_END

#endif // MATERIAL_H
