#include "ShaderPreprocessor.h"

#include "CodePreprocessor.h"

const String ShaderPreprocessor::c_version = "#version 130";

void ShaderPreprocessor::PreprocessCode(String *shaderSourceCode)
{
    CodePreprocessor::PreprocessCode(shaderSourceCode,
                                     {"Shaders/Include"});

    String &code = *shaderSourceCode;

    // Add the version. It must be the very first line
    code.Prepend(ShaderPreprocessor::c_version + "\n");
}

ShaderPreprocessor::ShaderPreprocessor()
{
}

