#ifndef SHADERPREPROCESSOR_H
#define SHADERPREPROCESSOR_H

#include "Bang/List.h"
#include "Bang/String.h"

class ShaderPreprocessor
{
public:
    /**
     * @brief Preprocess the passed shader source code.
     * For the moment, doing the needed recursive includes.
     * @param shaderSourceCode
     */
    static void PreprocessCode(String *shaderSourceCode);

protected:
    static const String GLSLVersionString;

private:
    ShaderPreprocessor();
};

#endif // SHADERPREPROCESSOR_H
