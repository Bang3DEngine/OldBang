#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "GLBindable.h"

class ShaderProgram : public GLIdable, public GLBindable
{
private:
    GLint lastBoundShaderProgramId;
    Shader *vshader, *fshader;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void BindVertexShader(Shader *vshader);
    void BindFragmentShader(Shader *fshader);

    bool Link();

    void Bind() const override;
    void UnBind() const override;

    Shader* GetVertexShader() const { return vshader; }
    Shader* GetFragmentShader() const { return fshader; }
};

#endif // SHADERPROGRAM_H
