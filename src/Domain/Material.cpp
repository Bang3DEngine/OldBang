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
            texture->Bind();
            shaderProgram->SetUniformTexture2D(ShaderContract::Uniform_Texture_Prefix + "0", texture, true);
        }
    }
}

void Material::UnBind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->UnBind();
        if(texture != nullptr)
        {
            texture->UnBind();
        }
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
