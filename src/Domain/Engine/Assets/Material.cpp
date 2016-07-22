#include "Material.h"

Material::Material() : Asset(),
                       m_diffuseColor(glm::vec4(0.0f)),
                       p_shaderProgram(nullptr)
{
}

Material::Material(const Material &m)
{
    SetDiffuseColor(m.GetDiffuseColor());

    //This is a copy of the pointers,
    // not the textures themselves
    m_namesToTextures = m.m_namesToTextures;

    //TODO: copy shaderProgram
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

        int slot = 1;
        for (auto it = m_namesToTextures.begin(); it != m_namesToTextures.end(); ++it)
        {
            std::string texName = it->first;
            Texture *t = it->second;
            if (t)
            {
                p_shaderProgram->SetUniformTexture(texName, t, slot, false); //Set the uniform with the texture slot
                t->Bind(); //Leave it bound
            }
            slot++;
        }

        p_shaderProgram->SetUniformVec4(ShaderContract::Uniform_Material_Diffuse_Color,
                                      m_diffuseColor, false);
    }
}

void Material::UnBind() const
{
    if (p_shaderProgram )
    {
        for (auto it = m_namesToTextures.begin(); it != m_namesToTextures.end(); ++it)
        {
            const Texture *t = it->second;
            if (t)
            {
                t->UnBind();
            }
        }
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
    FileWriter::Write(m_namesToTextures.size(), f);
    for (auto it = m_namesToTextures.begin(); it != m_namesToTextures.end(); ++it)
    {
        const std::string texName = it->first;
        Texture2D *tex = static_cast<Texture2D*>(it->second);
        const std::string texFilepath = tex->GetFilepath();
        FileWriter::Write(texName, f);
        FileWriter::WriteFilepath(texFilepath, f);
    }

    FileWriter::Write("</Material>", f);
}

void Material::Read(std::istream &f)
{
    std::string vshaderFilepath = FileReader::ReadString(f);
    std::string fshaderFilepath = FileReader::ReadString(f);
    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));

    int numTextures = FileReader::ReadInt(f);

    Logger_Log("Number of texs: " << numTextures);
    for(int i = 0; i < numTextures; ++i)
    {
        std::string texName = FileReader::ReadString(f);
        std::string texFilepath = FileReader::ReadString(f);
        Logger_Log("Name: " << texName);
        Logger_Log("Filepath: " << texFilepath);
        Texture2D *tex = AssetsManager::GetAsset<Texture2D>(texFilepath);
        if (tex)
        {
            SetTexture(texName, tex);
        }
    }

    glm::vec4 diffColor = FileReader::ReadVec4(f);
    SetDiffuseColor(diffColor);
}

void Material::SetShaderProgram(ShaderProgram *program)
{
    p_shaderProgram = program;
}

void Material::SetTexture(const std::string &nameInShader, Texture *texture)
{
    m_namesToTextures[nameInShader] = texture;
}

void Material::SetDiffuseColor(const glm::vec4 &diffuseColor)
{
    this->m_diffuseColor = diffuseColor;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return p_shaderProgram;
}

const Texture *Material::GetTexture(const std::string &nameInShader) const
{
    if (m_namesToTextures.find(nameInShader) != m_namesToTextures.end())
    {
        return m_namesToTextures[nameInShader];
    }

    return nullptr;
}

glm::vec4 Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
