#include "Bang/ShaderPreprocessor.h"

#include "Bang/List.h"
#include "Bang/Paths.h"
#include "Bang/CodePreprocessor.h"

USING_NAMESPACE_BANG

const String ShaderPreprocessor::GLSLVersionString = "#version 330 core";

void ShaderPreprocessor::PreprocessCode(String *shaderSourceCode)
{
    bool addVersion = !shaderSourceCode->BeginsWith("#version");
    CodePreprocessor::PreprocessCode(shaderSourceCode,
                                     {EPATH("Shaders/Include")});

    String &code = *shaderSourceCode;
    if (addVersion)
    {
        code.Prepend(ShaderPreprocessor::GLSLVersionString + "\n");
    }
}

ShaderPreprocessor::ShaderPreprocessor()
{
}
