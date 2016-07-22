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

std::string Material::GetTag() const
{
    return "Material";
}

void Material::WriteInternal(std::ostream &f) const
{
    Asset::WriteInternal(f);

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
    FileWriter::WriteFilepath(vsFile, f);
    FileWriter::WriteFilepath(fsFile, f);
    FileWriter::WriteVector4(m_diffuseColor, f);
    FileWriter::WriteInt(1, f);
    FileWriter::WriteFilepath(m_texture->GetFilepath(), f);
}

void Material::ReadInternal(std::istream &f)
{
    Asset::ReadInternal(f);

    std::string vshaderFilepath = FileReader::ReadString(f);
    std::string fshaderFilepath = FileReader::ReadString(f);
    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));

    int numTextures = FileReader::ReadInt(f);
    if(numTextures == 1)
    {
        std::string texAssetFilepath = FileReader::ReadString(f);
        Texture2D *texture = AssetsManager::GetAsset<Texture2D>(texAssetFilepath);
        m_shaderProgram->SetUniformTexture("B_texture_0", texture, false);
    }

    glm::vec4 diffColor = FileReader::ReadVec4(f);
    SetDiffuseColor(diffColor);
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
