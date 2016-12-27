#include "Material.h"

#include "Screen.h"
#include "Shader.h"
#include "Vector4.h"
#include "Texture.h"
#include "Texture2D.h"
#include "ShaderContract.h"
#include "ShaderProgram.h"

#include "AssetsManager.h"

Material::Material() : Asset()
{
    // TODO: Create shaderProgram asset and use AssetManager to load this
    SetShaderProgram(new ShaderProgram(
                         ShaderContract::Filepath_Shader_D2G_Default_VS,
                         ShaderContract::Filepath_Shader_D2G_Default_FS));
}

Material::Material(const Material &m)
{
    SetDiffuseColor(m.GetDiffuseColor());
    SetShaderProgram(new ShaderProgram(m.GetShaderProgram()->GetVertexShader()->GetFilepath(),
                                       m.GetShaderProgram()->GetFragmentShader()->GetFilepath()));
}

Material::~Material()
{
    if (m_shaderProgram)
    {
        delete m_shaderProgram;
    }
}

const String Material::GetFileExtensionStatic()
{
    return "bmat";
}

const String Material::GetFileExtension()
{
    return Material::GetFileExtensionStatic();
}

void Material::Bind() const
{
    if (m_shaderProgram)
    {
        m_shaderProgram->Bind();
        m_shaderProgram->SetUniformColor(ShaderContract::Uniform_Material_Diffuse_Color,
                                         m_diffuseColor, false);
        m_shaderProgram->SetUniformVec2("B_screen_size", Screen::GetSize(), false);
        m_shaderProgram->SetUniformFloat("B_material_shininess", m_shininess, false);

        if (m_texture)
        {
            m_shaderProgram->SetUniformFloat("B_hasTexture", 1, false);
            m_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
            m_shaderProgram->SetUniformFloat("B_alphaCuttoff", m_texture->GetAlphaCuttoff(), false);
        }
        else
        {
            m_shaderProgram->SetUniformFloat("B_hasTexture", 0, false);
            m_shaderProgram->SetUniformFloat("B_alphaCuttoff", -1.0f, false);
        }
    }
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

    String vshaderFilepath = xmlInfo->GetFilepath("VertexShader");
    String fshaderFilepath = xmlInfo->GetFilepath("FragmentShader");
    if (!m_shaderProgram ||
        !m_shaderProgram->GetVertexShader() || !m_shaderProgram->GetFragmentShader() ||
         vshaderFilepath != m_shaderProgram->GetVertexShader()->GetFilepath()   ||
         fshaderFilepath != m_shaderProgram->GetFragmentShader()->GetFilepath()
        )
    {
        SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));
    }

    int numTextures = xmlInfo->GetInt("TextureCount");
    if(numTextures == 1)
    {
        String texAssetFilepath = xmlInfo->GetString("Texture1");
        Texture2D *texture = AssetsManager::LoadAsset<Texture2D>(texAssetFilepath);
        SetTexture(texture);
    }

    SetDiffuseColor(xmlInfo->GetColor("DiffuseColor"));
    SetShininess(xmlInfo->GetFloat("Shininess"));
}

void Material::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Material");

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

    xmlInfo->SetFilepath("VertexShader", vsFile);
    xmlInfo->SetFilepath("FragmentShader", fsFile);

    if (m_texture)
    {
        xmlInfo->SetInt("TextureCount", 1);
        xmlInfo->SetFilepath("Texture1", m_texture->GetFilepath());
    }

    xmlInfo->SetColor("DiffuseColor", GetDiffuseColor());
    xmlInfo->SetFloat("Shininess", GetShininess());
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

float Material::GetShininess() const
{
    return m_shininess;
}

const Color& Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
