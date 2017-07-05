#ifndef G_MATERIAL_H
#define G_MATERIAL_H

#include "Bang/Color.h"
#include "Bang/Vector2.h"

class G_Texture2D;
class G_ShaderProgram;
class G_Material
{
public:
    G_Material();
    G_Material(const G_Material &m);
    virtual ~G_Material();

    void SetUvMultiply(const Vector2& uvMultiply);
    void SetShaderProgram(G_ShaderProgram *program);
    void SetTexture(const G_Texture2D *mtexture);
    void SetReceivesLighting(bool receivesLighting);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Color &diffuseColor);

    const Vector2&     GetUvMultiply() const;
    G_ShaderProgram*   GetShaderProgram() const;
    const G_Texture2D* GetTexture() const;
    bool ReceivesLighting() const;
    float GetShininess() const;
    const Color& GetDiffuseColor() const;

    virtual void Bind() const;
    virtual void UnBind() const;

protected:
    Color m_diffuseColor    = Color::White;
    float m_shininess       = 60.0f;
    bool m_receivesLighting = true;
    Vector2 m_uvMultiply    = Vector2::One;

    G_ShaderProgram *m_shaderProgram = nullptr;
    const G_Texture2D *m_texture     = nullptr;
};

#endif // G_MATERIAL_H
