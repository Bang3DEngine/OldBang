#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bang/Asset.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/ResourceHandle.h"
#include "Bang/ITextureChangedListener.h"
#include "Bang/IMaterialChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;
FORWARD class ShaderProgram;

class Material : public Asset,
                 public ITextureChangedListener,
                 public EventEmitter<IMaterialChangedListener>
{
    ASSET(Material)

public:
    void SetUvOffset(const Vector2& uvOffset);
    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(ShaderProgram *program);
    void SetTexture(Texture2D *texture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2& GetUvOffset() const;
    const Vector2& GetUvMultiply() const;
    ShaderProgram* GetShaderProgram() const;
    Texture2D* GetTexture() const;
    bool GetReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    virtual void Bind() const;
    virtual void UnBind() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // ITextureChangedListener
    void OnTextureChanged(const Texture *changedTexture) override;

    // Resource
    void Import(const Path &materialFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    RH<Texture2D> p_texture;
    RH<ShaderProgram> p_shaderProgram;

    Color m_diffuseColor    = Color::White;
    float m_shininess       = 60.0f;
    bool m_receivesLighting = true;
    Vector2 m_uvOffset      = Vector2::Zero;
    Vector2 m_uvMultiply    = Vector2::One;

    Material();
    virtual ~Material();

    void PropagateMaterialChanged();
};

NAMESPACE_BANG_END

#endif // MATERIAL_H
