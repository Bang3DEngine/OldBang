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
    if (m_shaderProgram )
    {
        m_shaderProgram->Bind();
        m_shaderProgram->SetUniformVec4(ShaderContract::Uniform_Material_Diffuse_Color,
                                        m_diffuseColor, false);
    }
}

void Material::UnBind() const
{
    if (m_shaderProgram)
    {
        m_shaderProgram->UnBind();
    }
}

void Material::ReadXMLNode(const XMLNode *xmlNode)
{
    Asset::ReadXMLNode(xmlNode);

    std::string vshaderFilepath = xmlNode->GetString("vertexShader");
    std::string fshaderFilepath = xmlNode->GetString("fragmentShader");
    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));

    int numTextures = xmlNode->GetInt("textureCount");
    if(numTextures == 1)
    {
        std::string texAssetFilepath = xmlNode->GetString("texture1");
        Texture2D *texture = AssetsManager::GetAsset<Texture2D>(texAssetFilepath);
        m_shaderProgram->SetUniformTexture("B_texture_0", texture, false);
    }

    glm::vec4 diffColor = xmlNode->GetVector4("diffuseColor");
    SetDiffuseColor(diffColor);
}

void Material::GetXMLNode(XMLNode *xmlNode) const
{
    Asset::GetXMLNode(xmlNode);
    xmlNode->SetTagName("Material");

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

    xmlNode->SetAttribute("vertexShader", vsFile);
    xmlNode->SetAttribute("fragmentShader", fsFile);
    xmlNode->SetAttribute("textureCount", 1);
    xmlNode->SetAttribute("texture1", m_texture->GetFilepath());
    xmlNode->SetAttribute("diffuseColor", m_diffuseColor);
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    m_shaderProgram = program;
}

void Material::SetTexture(Texture2D *texture)
{
    m_texture = texture;
    if(m_texture)
    {
        m_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
    }
}

void Material::SetDiffuseColor(const glm::vec4 &diffuseColor)
{
    this->m_diffuseColor = diffuseColor;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

const Texture *Material::GetTexture() const
{
    return m_texture;
}

glm::vec4 Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
