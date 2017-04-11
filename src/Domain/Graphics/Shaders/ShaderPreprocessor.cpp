#include "Bang/ShaderPreprocessor.h"

#include "Bang/CodePreprocessor.h"

const String ShaderPreprocessor::GLSLVersionString = "#version 130";

void ShaderPreprocessor::PreprocessCode(String *shaderSourceCode)
{
    CodePreprocessor::PreprocessCode(shaderSourceCode,
                                     {"Shaders/Include"});

    String &code = *shaderSourceCode;

    code.Prepend(ShaderPreprocessor::GLSLVersionString + "\n");
}

ShaderPreprocessor::ShaderPreprocessor()
{
}

