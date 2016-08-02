#include "Material.h"

Material::Material() : Asset()
{
}

Material::Material(const Material &m)
{
    SetDiffuseColor(m.GetDiffuseColor());
    SetShaderProgram(m.GetShaderProgram());
}

Material::~Material()
{

}

void Material::Bind() const
{
    if (m_shaderProgram)
    {
        m_shaderProgram->Bind();
        m_shaderProgram->SetUniformColor(ShaderContract::Uniform_Material_Diffuse_Color,
                                         m_diffuseColor, false);
        if (m_texture)
        {
            m_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
            m_shaderProgram->SetUniformFloat("B_alphaCuttoff", m_texture->GetAlphaCuttoff(), false);
        }
        else
        {
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

    std::string vshaderFilepath = xmlInfo->GetFilepath("VertexShader");
    std::string fshaderFilepath = xmlInfo->GetFilepath("FragmentShader");
    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));

    int numTextures = xmlInfo->GetInt("TextureCount");
    if(numTextures == 1)
    {
        std::string texAssetFilepath = xmlInfo->GetString("Texture1");
        Texture2D *texture = AssetsManager::LoadAsset<Texture2D>(texAssetFilepath);
        SetTexture(texture);
    }

    SetDiffuseColor(xmlInfo->GetColor("DiffuseColor"));
}

void Material::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Material");

    std::string vsFile =  "", fsFile = "";
    if (this->m_shaderProgram)
    {
        if (this->m_shaderProgram->GetVertexShader())
        {
            vsFile = this->m_shaderProgram->GetVertexShader()->GetFilepath();
        }

        if (this->m_shaderProgram->GetFragmentShader())
        {
            fsFile = this->m_shaderProgram->GetFragmentShader()->GetFilepath();
        }
    }

    xmlInfo->SetFilepath("VertexShader", vsFile);
    xmlInfo->SetFilepath("FragmentShader", fsFile);
    xmlInfo->SetInt("TextureCount", 1);
    xmlInfo->SetFilepath("Texture1", m_texture->GetFilepath());
    xmlInfo->SetColor("DiffuseColor", GetDiffuseColor());
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

void Material::SetDiffuseColor(const Color &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

const Texture *Material::GetTexture() const
{
    return m_texture;
}

const Color& Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
