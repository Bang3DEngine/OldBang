#include "Material.h"

#include "Screen.h"
#include "Shader.h"
#include "Vector4.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Persistence.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

Material::Material() : Asset()
{
    // TODO: Create shaderProgram asset and use AssetManager to load this
    SetShaderProgram(new ShaderProgram(
                      Persistence::ToAbsolute("Shaders/D2G_Default.vert", true),
                      Persistence::ToAbsolute("Shaders/D2G_Default.frag", true)
                     )
                    );
}

Material::Material(const Material &m)
{
    SetDiffuseColor(m.GetDiffuseColor());
    SetShaderProgram(
                new ShaderProgram(
                   m.GetShaderProgram()->GetVertexShader()->GetFilepath(),
                   m.GetShaderProgram()->GetFragmentShader()->GetFilepath()));
}

Material::~Material()
{
    if (m_shaderProgram)
    {
        delete m_shaderProgram;
    }
}

String Material::GetFileExtensionStatic()
{
    return "bmat";
}

String Material::GetFileExtension()
{
    return Material::GetFileExtensionStatic();
}

void Material::Bind() const
{
    ASSERT(m_shaderProgram);

    m_shaderProgram->Bind();
    m_shaderProgram->SetUniformColor("B_material_diffuse_color",
                                     m_diffuseColor, false);
    m_shaderProgram->SetUniformVec2("B_screen_size",
                                    Screen::GetSize(), false);
    m_shaderProgram->SetUniformFloat("B_material_shininess",
                                     m_shininess, false);
    m_shaderProgram->SetUniformFloat("B_material_receivesLighting",
                                 m_receivesLighting ? 1.0f : 0.0f, false);


    float alphaCutoff = m_texture ? m_texture->GetAlphaCutoff() : -1.0f;
    m_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
    m_shaderProgram->SetUniformFloat("B_alphaCutoff", alphaCutoff, false);
    m_shaderProgram->SetUniformFloat("B_hasTexture", m_texture ? 1 : 0,
                                     false);
}

void Material::UnBind() const
{
    if (m_shaderProgram)
    {
        m_shaderProgram->UnBind();
    }
}

void Material::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    SetDiffuseColor(xmlInfo->GetColor("DiffuseColor"));
    SetShininess(xmlInfo->GetFloat("Shininess"));
    SetReceivesLighting(xmlInfo->GetBool("ReceivesLighting"));

    String texAssetFilepath = xmlInfo->GetString("Texture");
    Texture2D *texture = AssetsManager::Load<Texture2D>(texAssetFilepath);
    SetTexture(texture);

    String vshaderFilepath = xmlInfo->GetFilepath("VertexShader");
    String fshaderFilepath = xmlInfo->GetFilepath("FragmentShader");
    if (!m_shaderProgram ||
        !m_shaderProgram->GetVertexShader() ||
        !m_shaderProgram->GetFragmentShader() ||
        vshaderFilepath != m_shaderProgram->GetVertexShader()->GetFilepath() ||
        fshaderFilepath != m_shaderProgram->GetFragmentShader()->GetFilepath()
        )
    {
        SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));
    }
}

void Material::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Material");

    xmlInfo->SetColor("DiffuseColor", GetDiffuseColor());
    xmlInfo->SetFloat("Shininess", GetShininess());
    xmlInfo->SetBool("ReceivesLighting", ReceivesLighting());

    String textureFilepath = m_texture ? m_texture->GetFilepath() : "";
    xmlInfo->SetFilepath("Texture", textureFilepath,
                         Texture2D::GetFileExtensionStatic());

    String vsFile =  "", fsFile = "";
    if (m_shaderProgram)
    {
        if (m_shaderProgram->GetVertexShader())
        {
            vsFile = m_shaderProgram->GetVertexShader()->GetFilepath();
        }

        if (m_shaderProgram->GetFragmentShader())
        {
            fsFile = m_shaderProgram->GetFragmentShader()->GetFilepath();
        }
    }
    xmlInfo->SetFilepath("VertexShader", vsFile, "vert");
    xmlInfo->SetFilepath("FragmentShader", fsFile, "frag");
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    m_shaderProgram = program;
}

void Material::SetTexture(const Texture2D *texture)
{
    m_texture = texture;
    if (m_texture)
    {
        m_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
    }
}

void Material::SetReceivesLighting(bool receivesLighting)
{
    m_receivesLighting = receivesLighting;
}

void Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

void Material::SetDiffuseColor(const Color &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

const Texture2D *Material::GetTexture() const
{
    return m_texture;
}

bool Material::ReceivesLighting() const
{
    return m_receivesLighting;
}

float Material::GetShininess() const
{
    return m_shininess;
}

const Color& Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
