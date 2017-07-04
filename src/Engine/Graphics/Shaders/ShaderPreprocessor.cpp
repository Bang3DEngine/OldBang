#include "Bang/ShaderPreprocessor.h"

#include "Bang/Paths.h"
#include "Bang/CodePreprocessor.h"

const String ShaderPreprocessor::GLSLVersionString = "#version 130";

void ShaderPreprocessor::PreprocessCode(String *shaderSourceCode)
{
    CodePreprocessor::PreprocessCode(shaderSourceCode,
                                     {EPATH("Shaders/Include")});

    String &code = *shaderSourceCode;

    code.Prepend(ShaderPreprocessor::GLSLVersionString + "\n");
}

ShaderPreprocessor::ShaderPreprocessor()
{
}

