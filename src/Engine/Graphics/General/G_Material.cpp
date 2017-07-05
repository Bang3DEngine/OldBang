#include "G_Material.h"

#include "Bang/Vector4.h"
#include "Bang/G_Screen.h"
#include "Bang/G_Shader.h"
#include "Bang/G_Texture.h"
#include "Bang/G_Texture2D.h"
#include "Bang/G_ShaderProgram.h"

G_Material::G_Material()
{
}

G_Material::G_Material(const G_Material &m)
{
    SetShaderProgram(m.GetShaderProgram());
    SetDiffuseColor(m.GetDiffuseColor());
    SetReceivesLighting(m.ReceivesLighting());
    SetShininess(m.GetShininess());
    SetTexture(m.GetTexture());
}

G_Material::~G_Material()
{
    if (m_shaderProgram) { delete m_shaderProgram; }
}

void G_Material::SetUvMultiply(const Vector2 &uvMultiply)
{
    m_uvMultiply = uvMultiply;
}

void G_Material::SetShaderProgram(G_ShaderProgram *program)
{
    m_shaderProgram = program;
}

void G_Material::SetTexture(const G_Texture2D *texture)
{
    m_texture = texture;
    if (m_texture)
    {
        G_ShaderProgram *sp = GetShaderProgram();
        sp->SetTexture("B_Texture0", m_texture);
    }
}

void G_Material::SetReceivesLighting(bool receivesLighting)
{
    m_receivesLighting = receivesLighting;
}

void G_Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

void G_Material::SetDiffuseColor(const Color &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

const Vector2 &G_Material::GetUvMultiply() const
{
    return m_uvMultiply;
}

G_ShaderProgram *G_Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

const G_Texture2D *G_Material::GetTexture() const
{
    return m_texture;
}

bool G_Material::ReceivesLighting() const
{
    return m_receivesLighting;
}

float G_Material::GetShininess() const
{
    return m_shininess;
}

const Color& G_Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}

void G_Material::Bind() const
{
    G_ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->Bind();

    sp->SetVec2("B_ScreenSize", G_Screen::GetSize());

    sp->SetVec2( "B_UvMultiply",               GetUvMultiply());
    sp->SetColor("B_MaterialDiffuseColor",     GetDiffuseColor());
    sp->SetFloat("B_MaterialShininess",        GetShininess());
    sp->SetFloat("B_MaterialReceivesLighting", ReceivesLighting() ? 1.0f : 0.0f);

    float alphaCutoff = GetTexture() ? GetTexture()->GetAlphaCutoff() : -1.0f;
    sp->SetTexture("B_Texture0",  GetTexture());
    sp->SetFloat("B_AlphaCutoff", alphaCutoff);
    sp->SetBool("B_HasTexture",   GetTexture() != nullptr);

    GL::ApplyContextToShaderProgram(sp);
}

void G_Material::UnBind() const
{
    G_ShaderProgram *sp = GetShaderProgram(); ENSURE(sp);
    sp->UnBind();
}
