#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"

class ShaderProgram : public GLIdable
{
private:
    Shader *vshader, *fshader;

public:
    ShaderProgram();
    ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    Shader* GetVertexShader() const { return vshader; };
    Shader* GetFragmentShader() const { return fshader; };
};

#endif // SHADERPROGRAM_H
