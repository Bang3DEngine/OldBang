#ifndef SHADERPROGRAMFACTORY_H
#define SHADERPROGRAMFACTORY_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Shader;
FORWARD class ShaderProgram;

class ShaderProgramFactory
{
public:
    static ShaderProgram* CreateEmptyProgram();
    static ShaderProgram* GetShaderProgram(const Path &vShaderPath,
                                           const Path &fShaderPath);
    static ShaderProgram* GetShaderProgram(Shader *vShader, Shader *fShader);

    ShaderProgramFactory() = delete;
    virtual ~ShaderProgramFactory() = delete;
};

NAMESPACE_BANG_END

#endif // SHADERPROGRAMFACTORY_H

