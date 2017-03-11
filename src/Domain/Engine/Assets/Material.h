#ifndef MATERIAL_H
#define MATERIAL_H

#include "Array.h"
#include "Asset.h"
#include "Color.h"
#include "IRenderAgent.h"

class Texture2D;
class ShaderProgram;
class Material : public Asset,
                 public IRenderAgent
{
public:
    Material();
    Material(const Material &m);
    virtual ~Material();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

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

    virtual void OnRenderingStarts(GameObject *go,
                                   ShaderProgram *sp) override;

    virtual void Read(const XMLNode *xmlInfo) override;
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
