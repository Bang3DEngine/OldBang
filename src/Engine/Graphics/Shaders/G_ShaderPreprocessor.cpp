#include "Bang/G_ShaderPreprocessor.h"

#include "Bang/Paths.h"
#include "Bang/CodePreprocessor.h"

const String G_ShaderPreprocessor::GLSLVersionString = "#version 130";

void G_ShaderPreprocessor::PreprocessCode(String *shaderSourceCode)
{
    CodePreprocessor::PreprocessCode(shaderSourceCode,
                                     {EPATH("Shaders/Include")});

    String &code = *shaderSourceCode;

    code.Prepend(G_ShaderPreprocessor::GLSLVersionString + "\n");
}

G_ShaderPreprocessor::G_ShaderPreprocessor()
{
}

