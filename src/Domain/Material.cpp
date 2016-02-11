#include "Material.h"

Material::Material() : shaderProgram(nullptr), texture(nullptr)
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
        if(texture != nullptr)
        {
            shaderProgram->SetUniformTexture2D(ShaderContract::Uniform_Texture_Prefix + "0", texture, false);
        }
    }
}

void Material::UnBind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->UnBind();
    }
}

void Material::SetShaderProgram(const ShaderProgram *program)
{
    shaderProgram = program;
}

void Material::SetTexture(const Texture *texture)
{
    this->texture = texture;
}

const ShaderProgram *Material::GetShaderProgram() const
{
    return shaderProgram;
}

const Texture *Material::GetTexture() const
{
    return texture;
}
