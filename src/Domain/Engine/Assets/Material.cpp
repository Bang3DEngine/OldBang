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
    if (p_shaderProgram )
    {
        p_shaderProgram->Bind();
        p_shaderProgram->SetUniformVec4(ShaderContract::Uniform_Material_Diffuse_Color,
                                        m_diffuseColor, false);
    }
}

void Material::UnBind() const
{
    if (p_shaderProgram)
    {
        p_shaderProgram->UnBind();
    }
}

void Material::Write(std::ostream &f) const
{
    FileWriter::Write("<Material>", f);
    std::string vsFile =  "", fsFile = "";
    if (this->p_shaderProgram)
    {
        if (this->p_shaderProgram->GetVertexShader())
        {
            vsFile = this->p_shaderProgram->GetVertexShader()->GetFilepath();
        }

        if (this->p_shaderProgram->GetFragmentShader())
        {
            fsFile = this->p_shaderProgram->GetFragmentShader()->GetFilepath();
        }
    }
    FileWriter::Write(vsFile, f);
    FileWriter::Write(fsFile, f);
    FileWriter::Write(m_diffuseColor, f);
    FileWriter::Write(1, f);
    FileWriter::Write(m_texture->GetFilepath(), f);

    FileWriter::Write("</Material>", f);
}

void Material::Read(std::istream &f)
{
    std::string vshaderFilepath = FileReader::ReadString(f);
    std::string fshaderFilepath = FileReader::ReadString(f);
    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));

    int numTextures = FileReader::ReadInt(f);
    std::string texAssetFilepath = FileReader::ReadString(f);
    Texture2D *texture = AssetsManager::GetAsset<Texture2D>(texAssetFilepath);
    p_shaderProgram->SetUniformTexture("B_texture_0", texture, false);


    glm::vec4 diffColor = FileReader::ReadVec4(f);
    SetDiffuseColor(diffColor);
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    p_shaderProgram = program;
}

void Material::SetTexture(Texture2D *texture)
{
    m_texture = texture;
    if(m_texture)
    {
        p_shaderProgram->SetUniformTexture("B_texture_0", m_texture, false);
    }
}

void Material::SetDiffuseColor(const glm::vec4 &diffuseColor)
{
    this->m_diffuseColor = diffuseColor;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return p_shaderProgram;
}

const Texture *Material::GetTexture() const
{
    return m_texture;
}

glm::vec4 Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
