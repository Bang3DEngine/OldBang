#ifndef CODEPREPROCESSOR_H
#define CODEPREPROCESSOR_H

#include "Array.h"
#include "String.h"

class CodePreprocessor
{
public:

    /**
     * @brief Preprocess the passed source code.
     * For the moment, doing the needed recursive includes.
     * @param srcCode
     */
    static void PreprocessCode(String *srcCode,
                               const List<String> &includePaths);

protected:
    /**
     * @brief Given an include directive, returns
     * the String that it should be replaced with.
     * @param includeDirective '#include "myShaderFile.frag"'
     * @return The String that the include directive
     * should be replaced with.
     */
    static String GetIncludeReplacementString(const String &includeDirective,
                                              int includeDirectiveLine,
                                              const List<String> &includePaths);

private:
    CodePreprocessor();

};

#endif // CODEPREPROCESSOR_H
