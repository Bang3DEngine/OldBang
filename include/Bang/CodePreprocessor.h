#ifndef CODEPREPROCESSOR_H
#define CODEPREPROCESSOR_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class CodePreprocessor
{
public:

    static void PreprocessCode(String *srcCode,
                               const List<Path> &includePaths);

    static List<String> GetSourceIncludeDirectives(const String &srcCode);
    static List<Path> GetSourceIncludePaths(const Path &srcPath,
                                            const List<Path> &includePaths);
    static List<Path> GetSourceIncludePaths(const String &srcCode,
                                            const List<Path> &includePaths);
protected:

    static String GetIncludeString(const String &includeDirective);
    static Path GetIncludePath(const String &includeDirective,
                               const List<Path> &includePaths);
    static String GetIncludeContents(const String &includeDirective,
                                     const List<Path> &includePaths);

private:
    CodePreprocessor();
};

NAMESPACE_BANG_END

#endif // CODEPREPROCESSOR_H
