#ifndef MATERIAL_H
#define MATERIAL_H

#include <ShaderProgram.h>

class Material
{
friend class MeshRenderer;

private:
    const ShaderProgram *shaderProgram;

public:
    Material();
    ~Material();

    void SetShaderProgram(const ShaderProgram *program);
    const ShaderProgram* GetShaderProgram() const { return shaderProgram; }
};

#endif // MATERIAL_H
