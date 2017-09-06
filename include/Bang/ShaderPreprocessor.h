#ifndef SHADERPREPROCESSOR_H
#define SHADERPREPROCESSOR_H

#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class ShaderPreprocessor
{
public:
    static void PreprocessCode(String *shaderSourceCode);

protected:
    static const String GLSLVersionString;

private:
    ShaderPreprocessor();
};

NAMESPACE_BANG_END

#endif // SHADERPREPROCESSOR_H
