#ifndef SHADERPREPROCESSOR_H
#define SHADERPREPROCESSOR_H


#include <algorithm>

#include "Bang.h"

#include "File.h"
#include "Array.h"
#include "StringUtils.h"
#include "ShaderContract.h"

class ShaderPreprocessor
{
private:
    ShaderPreprocessor();

    static const Array<String> c_includePaths;

    /**
     * @brief Given an include directive, returns
     * the String that it should be replaced with.
     * @param includeDirective '#include "myShaderFile.frag"'
     * @return The String that the include directive
     * should be replaced with.
     */
    static String GetIncludeReplacementString(const String &includeDirective,
                                              int includeDirectiveLine);

public:

    /**
     * @brief Preprocess the passed shader source code.
     * For the moment, doing the needed recursive includes.
     * @param shaderSourceCode
     */
    static void PreprocessShaderCode(String *shaderSourceCode);
};

#endif // SHADERPREPROCESSOR_H
