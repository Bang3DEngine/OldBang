#include "Material.h"

Material::Material() : Asset(),
                       diffuseColor(glm::vec4(1.0f, 0.0f, 1.0f, 0.5f)),
                       shaderProgram(nullptr)
{
}

Material::~Material()
{

}

void Material::Bind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->Bind();

        for(unsigned int i = 0; i < textures.size(); ++i)
        {
            const Texture *t = textures[i];
            if(t != nullptr)
            {
                std::string texName = ShaderContract::Uniform_Texture_Prefix + std::to_string(i);
                shaderProgram->SetUniformTexture(texName, t, false); //Set the uniform with the texture slot
                t->Bind(); //Leave it bound
            }
        }

        shaderProgram->SetUniformVec4(ShaderContract::Uniform_Material_Diffuse_Color,
                                      diffuseColor, false);
    }
}

void Material::UnBind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->UnBind();
        for(unsigned int i = 0; i < textures.size(); ++i)
        {
            if(textures[i] != nullptr)
                textures[i]->UnBind();
        }
    }
}

void Material::Write(std::ostream &f) const
{

}

void Material::Read(std::istream &f)
{
    std::string vshaderFilepath = FileReader::ReadString(f);
    std::string fshaderFilepath = FileReader::ReadString(f);
    std::string texFilepath = FileReader::ReadString(f);

    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));
    Texture2D *tex = AssetsManager::GetAsset<Texture2D>( texFilepath );
    tex->SetTextureSlot(0);
    SetTexture(tex);
}

void Material::SetShaderProgram(const ShaderProgram *program)
{
    shaderProgram = program;
}

void Material::SetTexture(const Texture *texture, unsigned int index)
{
    while(textures.size() <= index) textures.push_back(nullptr);
    textures[index] = texture;
}

void Material::SetDiffuseColor(const glm::vec4 &diffuseColor)
{
    this->diffuseColor = diffuseColor;
}

const ShaderProgram *Material::GetShaderProgram() const
{
    return shaderProgram;
}

const Texture *Material::GetTexture(unsigned int index) const
{
    if(index >= textures.size()) return nullptr;
    return textures[index];
}

glm::vec4 Material::GetDiffuseColor() const
{
    return diffuseColor;
}
