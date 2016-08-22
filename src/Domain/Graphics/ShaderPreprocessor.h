#ifndef SHADERPREPROCESSOR_H
#define SHADERPREPROCESSOR_H


#include <algorithm>

#include "Bang.h"

#include "StringUtils.h"
#include "ShaderContract.h"

class ShaderPreprocessor
{
private:
    ShaderPreprocessor();

public:
    static void PreprocessShaderCode(String *shaderSourceCode);
};

#endif // SHADERPREPROCESSOR_H
