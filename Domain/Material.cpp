#include "Material.h"

Material::Material() : shaderProgram(nullptr)
{
}

void Material::SetShaderProgram(const ShaderProgram *program)
{
    shaderProgram = program;
}
