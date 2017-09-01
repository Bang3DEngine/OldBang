#ifndef CODEPREPROCESSOR_H
#define CODEPREPROCESSOR_H

#include "Bang/Bang.h"

class CodePreprocessor
{
public:

    /**
     * @brief Preprocess the passed source code.
     * For the moment, doing the needed recursive includes.
     * @param srcCode
     */
    static void PreprocessCode(String *srcCode,
                               const List<Path> &includePaths);

protected:
    /**
     * @brief Given an include directive, returns
     * the String that it should be replaced with.
     * @param includeDirective '#include "Bang/myShaderFile.frag"'
     * @return The String that the include directive
     * should be replaced with.
     */
    static String GetIncludeContents(const String &includeDirective,
                                              const List<Path> &includePaths);

private:
    CodePreprocessor();

};

#endif // CODEPREPROCESSOR_H
